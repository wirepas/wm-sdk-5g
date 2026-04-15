# Boards compatible with this app 
TARGET_BOARDS := pca10153 pca10165 pca10171 pca20049 pca20064 pca20065 
#
# Network default settings configuration
#

# If this section is removed, node has to be configured in
# a different way
default_network_address ?= 0x1012EE
default_network_channel ?= 2
# For running this application with network encryption, the following key definitions may be uncommented and
# filled with random data (exactly 16 bytes each). Also `allow_insecure_key_injection` needs to be set
# to 'yes'. Note that the keys end up as plaintext in device flash with this mechanism. Not for
# production use!
#
# For production use, use a secure provisioning method or refer to the app_setup library in the SDK.

default_network_cipher_key ?= 0x10,0x0f,0x0e,0x0d,0x0c,0x0b,0x0a,0x09,0x08,0x07,0x06,0x05,0x04,0x03,0x02,0x01
default_network_authen_key ?= 0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,0x10
allow_insecure_key_injection=yes

#
# App specific configuration
#

# Define a specific application area_id
app_specific_area_id=0x8A2336

# App version
app_major=0
app_minor=0
app_maintenance=1
app_development=0
