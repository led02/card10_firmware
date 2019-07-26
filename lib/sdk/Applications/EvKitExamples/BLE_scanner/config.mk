# Specifies whether the legacy 4.2 api should be used.
# 1 by default.
# Set to 1 to use 4.2 advertisements.
# Set to 0 to use 5.0 extended advertisements.
BTLE_APP_USE_LEGACY_API?=1

# Specifies whether scan requests should be transmitted.
BTLE_APP_USE_ACTIVE_SCANNING?=1

# UART used for console output.
# 1 by default.
CONSOLE_UART?=1

# Run link layer using SDMA.
ENABLE_SDMA?=0
