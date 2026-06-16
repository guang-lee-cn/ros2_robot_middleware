# Camera image — resolution, encoding, and pixel data
std_msgs/Header header

uint32 height                 # rows
uint32 width                  # columns
string encoding               # e.g. "rgb8", "mono8"
uint8[] data                  # raw pixel bytes
uint32 step                   # 每行字节数, width*channels (+padding)
bool   is_bigendian           # 字节序, 0=little-endian
