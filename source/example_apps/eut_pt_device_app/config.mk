# Boards compatible with this app 
TARGET_BOARDS := pca10153 

# Network address and channel
default_network_address ?= 0xB33AC0
default_network_channel ?= 2

# Define periodic interval for sending UL packet
# Default interval = 1000ms
send_ul_period_ms ?= 1000

# Define a specific application area_id
app_specific_area_id=0x846B74

# App version
app_major=$(sdk_major)
app_minor=$(sdk_minor)
app_maintenance=$(sdk_maintenance)
app_development=$(sdk_development)
