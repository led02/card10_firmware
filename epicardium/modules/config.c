#include "modules/log.h"
#include "modules/config.h"
#include "modules/filesystem.h"

#include <assert.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define CONFIG_MAX_LINE_LENGTH 80

enum OptionType {
	OptionType_Boolean,
	OptionType_Int,
	OptionType_Float,
	OptionType_String,
};

struct config_option {
	const char *name;
	enum OptionType type;
	union {
		bool boolean;
		long integer;
		double floating_point;
		char *string;
	} value;
};

static struct config_option s_options[_EpicOptionCount] = {
/* clang-format off */
	#define INIT_Boolean(v)  { .boolean        = (v) }
	#define INIT_Int(v)      { .integer        = (v) }
	#define INIT_Float(v)    { .floating_point = (v) }
	#define INIT_String(v)   { .string         = (v) }
	#define INIT_(tp, v)     INIT_ ## tp (v)
	#define INIT(tp, v)      INIT_ (tp, v)

	#define CARD10_SETTING(identifier, spelling, tp, default_value)         \
		[Option ## identifier] = { .name  = (spelling),                 \
		                           .type  = OptionType_ ## tp,          \
		                           .value = INIT(tp, (default_value)) },

	#include "modules/config.def"
	/* clang-format on */
};

static struct config_option *findOption(const char *key)
{
	for (int i = 0; i < _EpicOptionCount; ++i) {
		if (!strcmp(key, s_options[i].name)) {
			return &s_options[i];
		}
	}
	return NULL;
}

static bool set_bool(struct config_option *opt, const char *value)
{
	bool val;
	if (!strcmp(value, "1")) {
		val = true;
	} else if (!strcmp(value, "true")) {
		val = true;
	} else if (!strcmp(value, "0")) {
		val = false;
	} else if (!strcmp(value, "false")) {
		val = false;
	} else {
		return false;
	}
	opt->value.boolean = val;
	LOG_DEBUG(
		"card10.cfg",
		"setting '%s' to %s",
		opt->name,
		val ? "true" : "false"
	);
	return true;
}

static bool set_int(struct config_option *opt, const char *value)
{
	char *endptr;
	size_t len = strlen(value);
	int v      = strtol(value, &endptr, 0);
	if (endptr != (value + len)) {
		return false;
	}
	opt->value.integer = v;
	LOG_DEBUG("card10.cfg", "setting '%s' to %d (0x%08x)", opt->name, v, v);
	return true;
}

static bool set_float(struct config_option *opt, const char *value)
{
	char *endptr;
	size_t len = strlen(value);
	double v   = strtod(value, &endptr);
	if (endptr != (value + len)) {
		return false;
	}
	opt->value.floating_point = v;
	LOG_DEBUG("card10.cfg", "setting '%s' to %f", opt->name, v);
	return true;
}

const char *elide(const char *str)
{
	static char ret[21];
	size_t len = strlen(str);
	if (len <= 20) {
		return str;
	}
	strncpy(ret, str, 17);
	ret[17] = '.';
	ret[18] = '.';
	ret[19] = '.';
	ret[20] = '\0';
	return ret;
}

static bool set_string(struct config_option *opt, const char *value)
{
	//this leaks, but the lifetime of these ends when epicardium exits, so...
	char *leaks       = strdup(value);
	opt->value.string = leaks;
	LOG_DEBUG("card10.cfg", "setting '%s' to %s", opt->name, elide(leaks));
	return true;
}

static void configure(const char *key, const char *value, int lineNumber)
{
	struct config_option *opt = findOption(key);
	if (!opt) {
		//invalid key
		LOG_WARN(
			"card10.cfg",
			"line %d: ignoring unknown option '%s'",
			lineNumber,
			key
		);
		return;
	}
	bool ok = false;
	switch (opt->type) {
	case OptionType_Boolean:
		ok = set_bool(opt, value);
		break;
	case OptionType_Int:
		ok = set_int(opt, value);
		break;
	case OptionType_Float:
		ok = set_float(opt, value);
		break;
	case OptionType_String:
		ok = set_string(opt, value);
		break;
	default:
		assert(0 && "unreachable");
	}
	if (!ok) {
		LOG_WARN(
			"card10.cfg",
			"line %d: ignoring invalid value '%s' for option '%s'",
			lineNumber,
			value,
			key
		);
	}
}

static void doline(char *line, char *eol, int lineNumber)
{
	//skip leading whitespace
	while (*line && isspace((int)*line))
		++line;

	char *key = line;
	if (*key == '#') {
		//skip comments
		return;
	}

	char *eq = strchr(line, '=');
	if (!eq) {
		if (*key) {
			LOG_WARN(
				"card10.cfg",
				"line %d (%s): syntax error",
				lineNumber,
				elide(line)
			);
		}
		return;
	}

	char *e_key = eq - 1;
	//skip trailing whitespace in key
	while (e_key > key && isspace((int)*e_key))
		--e_key;
	e_key[1] = '\0';
	if (*key == '\0') {
		LOG_WARN("card10.cfg", "line %d: empty key", lineNumber);
		return;
	}

	char *value = eq + 1;
	//skip leading whitespace
	while (*value && isspace((int)*value))
		++value;

	char *e_val = eol - 1;
	//skip trailing whitespace
	while (e_val > value && isspace((int)*e_val))
		--e_val;
	if (*value == '\0') {
		LOG_WARN(
			"card10.cfg",
			"line %d: empty value for option '%s'",
			lineNumber,
			key
		);
		return;
	}

	configure(key, value, lineNumber);
}

bool config_get_boolean(enum EpicConfigOption option)
{
	struct config_option *opt = &s_options[option];
	assert(opt->type == OptionType_Boolean);
	return opt->value.boolean;
}

long config_get_integer(enum EpicConfigOption option)
{
	struct config_option *opt = &s_options[option];
	assert(opt->type == OptionType_Int);
	return opt->value.integer;
}

double config_get_float(enum EpicConfigOption option)
{
	struct config_option *opt = &s_options[option];
	assert(opt->type == OptionType_Float);
	return opt->value.floating_point;
}

const char *config_get_string(enum EpicConfigOption option)
{
	struct config_option *opt = &s_options[option];
	assert(opt->type == OptionType_String);
	return opt->value.string;
}

void load_config(void)
{
	LOG_DEBUG("card10.cfg", "loading...");
	int fd = epic_file_open("card10.cfg", "r");
	if (fd < 0) {
		LOG_DEBUG(
			"card10.cfg",
			"loading failed: %s (%d)",
			strerror(-fd),
			fd
		);
		return;
	}
	char buf[CONFIG_MAX_LINE_LENGTH + 1];
	int lineNumber = 0;
	int nread;
	do {
		nread = epic_file_read(fd, buf, CONFIG_MAX_LINE_LENGTH);
		if (nread < CONFIG_MAX_LINE_LENGTH) {
			//add fake EOL to ensure termination
			buf[nread++] = '\n';
		}
		//zero-terminate buffer
		buf[nread]   = '\0';
		char *line   = buf;
		char *eol    = NULL;
		int last_eol = 0;
		while (line) {
			//line points one character past the las (if any) '\n' hence '- 1'
			last_eol = line - buf - 1;
			eol      = strchr(line, '\n');
			++lineNumber;
			if (eol) {
				*eol = '\0';
				doline(line, eol, lineNumber);
				line = eol + 1;
			} else {
				if (line == buf) {
					//line did not fit into buf
					LOG_WARN(
						"card10.cfg",
						"line:%d: too long - aborting",
						lineNumber
					);
					return;
				} else {
					int seek_back = last_eol - nread;
					LOG_DEBUG(
						"card10.cfg",
						"nread, last_eol, seek_back: %d,%d,%d",
						nread,
						last_eol,
						seek_back
					);
					assert(seek_back <= 0);
					if (seek_back) {
						int rc = epic_file_seek(
							fd,
							seek_back,
							SEEK_CUR
						);
						if (rc < 0) {
							LOG_ERR("card10.cfg",
								"seek failed, aborting");
							return;
						}
						char newline;
						rc = epic_file_read(
							fd, &newline, 1
						);
						if (rc < 0 || newline != '\n') {
							LOG_ERR("card10.cfg",
								"seek failed, aborting");
							LOG_DEBUG(
								"card10.cfg",
								"seek failed at read-back of newline: rc: %d read: %d",
								rc,
								(int)newline
							);
							return;
						}
					}
					break;
				}
			}
		}
	} while (nread == sizeof(buf));
	epic_file_close(fd);
}
