# Boards compatible with this app 
TARGET_BOARDS := pca10153 pca10165 pca10171 pca20049 pca20064 pca20065 
# Define a specific application area_id
app_specific_area_id=0x8cad1b

# App version
app_major=1
app_minor=0
app_maintenance=0
app_development=0

# Enable multi-instance UART driver (set to yes for multi-UART support)
uart_multi_instance?=no

usart_multi_tx_buffer_size?=256
usart_multi_rx_buffer_size?=256
usart_multi_max_instances?=1
