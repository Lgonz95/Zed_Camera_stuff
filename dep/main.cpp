//Author Llenard Gonzalo
//Returns Distance values

#include <stdio.h>
#include <string.h>


#include <sl/Camera.hpp>



using namespace sl;
using namespace std;


int main(int argc, char **argv) {

	bool on = true;
    // Create a ZED camera object
    Camera zed;

    // Set configuration parameters
    InitParameters init_params;
    init_params.depth_mode = DEPTH_MODE_ULTRA; // Difference depth modes from performance to ultra affect accuracy
    init_params.coordinate_units = UNIT_METER; // Default uses mm --> converting to metres
	init_params.depth_minimum_distance = 0.15; //minimum perception distance
	zed.setDepthMaxRangeValue(40);  //Max Perpection distance (Spec is 40m)


    // Open the camera
    ERROR_CODE err = zed.open(init_params);
    if (err != SUCCESS)
        exit(-1);

    // Set runtime parameters after opening the camera
    RuntimeParameters runtime_parameters;
    runtime_parameters.sensing_mode = SENSING_MODE_STANDARD; // Use STANDARD sensing mode
	//point cloud
    sl::Mat image, depth, point_cloud;

    while (on == true) {
        // A new image is available if grab() returns SUCCESS
        if (zed.grab(runtime_parameters) == SUCCESS) {
            // Retrieve left image
            zed.retrieveImage(image, VIEW_LEFT);
            
            zed.retrieveMeasure(depth, MEASURE_DEPTH);
           
            zed.retrieveMeasure(point_cloud, MEASURE_XYZRGBA);

            // Get and print distance value in m at the center of the image
            // We measure the distance camera - object using Euclidean distance
            int x = image.getWidth() / 2;
            int y = image.getHeight() / 2;
            sl::float4 point_cloud_value;
            point_cloud.getValue(x, y, &point_cloud_value);

            float distance = sqrt(point_cloud_value.x * point_cloud_value.x + point_cloud_value.y * point_cloud_value.y + point_cloud_value.z * point_cloud_value.z);
            printf("Distance to Camera at (%d, %d): %f m\n", x, y, distance);

           
        }
    }
    // Close the camera
    zed.close();
    return 0;
}
