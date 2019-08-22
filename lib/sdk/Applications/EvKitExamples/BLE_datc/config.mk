# Specifies whether the legacy 4.2 api should be used.
# 1 by default.
# Set to 1 to use 4.2 advertisements.
# Set to 0 to use 5.0 extended advertisements.
BTLE_APP_USE_LEGACY_API?=1

# Specifies whether scan requests should be transmitted.
BTLE_APP_USE_ACTIVE_SCANNING?=1

# Specifies whether syncing to periodic advertisements is enabled.
BTLE_APP_ENABLE_PERIODIC?=0

# Specifies whether whitelisting should be enabled.
BTLE_APP_ENABLE_WHITELIST?=0

# Run link layer using SDMA.
ENABLE_SDMA?=0
