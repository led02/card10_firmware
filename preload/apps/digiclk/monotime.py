import utime as _utime

_offset_ms = 0


def time_monotonic():
    return _utime.time() + _offset_ms // 1000


def time_monotonic_ms():
    return _utime.time_ms() + _offset_ms


def sleep(s):
    return _utime.sleep(s)


def sleep_ms(ms):
    return _utime.sleep_ms(ms)


def sleep_us(us):
    return _utime.sleep_us(us)


def time():
    return _utime.time()


def time_ms():
    return _utime.time_ms()


def set_time(t):
    global _offset_ms

    cur_t = _utime.time_ms()
    _utime.set_time(t)
    new_t = _utime.time_ms()

    diff = cur_t - new_t
    _offset_ms += diff


def set_unix_time(t):
    global _offset_ms

    cur_t = _utime.time_ms()
    _utime.set_unix_time(t)
    new_t = _utime.time_ms()

    diff = cur_t - new_t
    _offset_ms += diff


def localtime(s=None):
    if s != None:
        return _utime.localtime(s)
    else:
        return _utime.localtime()


def mktime(t):
    return _utime.mktime(t)


def alarm(s, cb=None):
    if cb != None:
        return _utime.alarm(s, cb)
    else:
        return _utime.alarm(s)
