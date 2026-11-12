# Boards compatible with this app 
TARGET_BOARDS := pca10153 pca10165 pca10171 pca20049 pca20064 pca20065 
#
# App specific configuration
#

# Application area ID
app_specific_area_id ?= 0x82F599

# App version
app_major ?= 3
app_minor ?= 0
app_maintenance ?= 0
app_development ?= 0




usart_multi_tx_buffer_size?=256
usart_multi_rx_buffer_size?=256
usart_multi_max_instances?=1