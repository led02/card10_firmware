# Specifies whether the legacy 4.2 api should be used.
# 1 by default.
# Set to 1 to use 4.2 advertisements.
# Set to 0 to use 5.0 extended advertisements.
BTLE_APP_USE_LEGACY_API?=1

# Specifies whether periodic advertisements are used.
BTLE_APP_ENABLE_PERIODIC?=0

# Specifies the advertisement type used.
# Unspecified -> Use default (connectable).
# DM_ADV_SCAN_UNDIRECT -> Non-connectable scannable
# DM_ADV_NONCONN_UNDIRECT -> Non-connectable non-scannable
BTLE_APP_ADV_TYPE?=
#BTLE_APP_ADV_TYPE?=DM_ADV_NONCONN_UNDIRECT
#BTLE_APP_ADV_TYPE?=DM_ADV_SCAN_UNDIRECT

# Run link layer using SDMA.
ENABLE_SDMA?=0
