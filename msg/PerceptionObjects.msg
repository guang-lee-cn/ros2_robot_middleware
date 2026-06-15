# Fused perception output — detected objects with positions
std_msgs/Header header

string[]  object_ids           # unique identifier per object
float32[] positions_x          # x coordinates in base_link frame
float32[] positions_y          # y coordinates in base_link frame
uint32    num_objects          # count of detected objects
