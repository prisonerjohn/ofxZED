/*
 * SOFTWARE LICENSE
 * BY USING YOUR CAMERA YOU AGREE TO THIS SOFTWARE LICENSE. BEFORE SETTING IT UP,
 * PLEASE READ THIS SOFTWARE LICENSE CAREFULLY. IF YOU DO NOT ACCEPT THIS
 * SOFTWARE LICENSE, DO NOT USE YOUR CAMERA. RETURN IT TO UNUSED TO STEREOLABS
 * FOR A REFUND. Contact STEREOLABS at support@stereolabs.com
 *
 * 1. Definitions
 *
 * "Authorized Accessory" means a STEREOLABS branded ZED or ZED Mini, and a STEREOLABS
 * licensed, third party branded, ZED hardware accessory whose packaging bears the official
 * "Licensed for ZED" logo. The ZED camera and the ZED Mini Camera are Authorized Accessories
 * solely for purpose of this Software license.
 * "Software" means the Software Development Kit, pre-installed in the ZED USB flash drive
 * included in the ZED packaging, and including any updates STEREOLABS may make available from
 * time to time.
 * "Unauthorized Accessories" means all hardware accessories other than an Authorized Accessory.
 * "Unauthorized Software" means any software not distributed by STEREOLABS.
 * "You" means the user of a ZED or ZED Mini Camera.
 *
 * 2. License
 *
 * a. The Software is licensed to You, not sold. You are licensed to use the
 * Software only as pre-installed in Your ZED USB flash drive, and updated by
 * STEREOLABS from time to time. You may not copy or reverse engineer the Software.
 * b. As conditions to this Software license, You agree that:
 *	i. You will use Your Software with ZED or ZED Mini Camera only and not with any
 *      other device (including). You will not use Unauthorized Accessories. They may
 *      not work or may stop working permanently after a Software update.
 *	ii. You will not use or install any Unauthorized Software. If You do, Your ZED
 *       or ZED Mini Camera may stop working permanently at that time or after a later
 *       Software update.
 *	iii. You will not attempt to defeat or circumvent any Software technical limitation,
 *        security, or anti-piracy system. If You do, Your ZED or ZED Mini Camera may stop
 *        working permanently at that time or after a later Software update.
 *	iv. STEREOLABS may use technical measures, including Software updates, to limit use
 *       of the Software to the ZED or ZED Mini Camera, to prevent use of Unauthorized
 *       Accessories, and to protect the technical limitations, security and anti-piracy
 *       systems in the ZED or ZED Mini Camera.
 *	v. STEREOLABS may update the Software from time to time without further notice to You,
 *      for example, to update any technical limitation, security, or anti-piracy system.
 *
 * 3. Warranty
 * The Software is covered by the Limited Warranty for Your ZED or ZED Mini Camera, and
 * STEREOLABS gives no other guarantee, warranty, or condition for the Software. No one
 * else may give any guarantee, warranty, or condition on STEREOLABS's behalf.
 *
 * 4. EXCLUSION OF CERTAIN DAMAGES
 * STEREOLABS IS NOT RESPONSIBLE FOR ANY INDIRECT, INCIDENTAL, SPECIAL, OR CONSEQUENTIAL
 * DAMAGES; ANY LOSS OF DATA, PRIVACY, CONFIDENTIALITY, OR PROFITS; OR ANY INABILITY TO
 * USE THE SOFTWARE. THESE EXCLUSIONS APPLY EVEN IF STEREOLABS HAS BEEN ADVISED OF THE
 * POSSIBILITY OF THESE DAMAGES, AND EVEN IF ANY REMEDY FAILS OF ITS ESSENTIAL PURPOSE.
 *
 * 5. Choice of Law
 * French law governs the interpretation of this Software license and any claim that
 * STEREOLABS has breached it, regardless of conflict of law principles.
 *
 */

#ifndef __CAMERA_HPP__
#define __CAMERA_HPP__

#include "sl_core/utils/Core.hpp"
#include "sl_core/mapping/Mesh.hpp"
#include "sl_zed/defines.hpp"
#include <cuda.h>

// Stereolabs namespace
namespace sl {

    ///@cond
    class SL_SDK_EXPORT Camera;
    ///@endcond

    /**
    \class InitParameters
    \ingroup Video_group
    \brief Holds the options used to initialize the \ref Camera object. \n
           Once passed to the \ref Camera::open() function, these settings will be set for the entire execution life time of the \ref Camera. \n
           You can get further information in the detailed description bellow.\n

    This structure allows you to select multiple parameters for the \ref Camera such as the selected camera, its resolution, depth mode, coordinate system, and unit, of measurement.
    Once filled with the desired options, it should be passed to the \ref Camera::open function.
    
    \code
    #include <sl/Camera.hpp>
    using namespace sl;
    int main(int argc, char **argv) {
        Camera zed; // Create a ZED camera object

        InitParameters init_params; // Set initial parameters
        init_params.sdk_verbose = false; // Disable verbose mode
        init_params.camera_resolution = RESOLUTION_HD1080; // Use HD1080 video mode
        init_params.camera_fps = 30; // Set fps at 30
        // Other parameters are left to their default values

        // Open the camera
        ERROR_CODE err = zed.open(init_params);
        if (err != SUCCESS)
            exit(-1);

        // Close the camera
        zed.close();
        return 0;
    }
    \endcode
    With its default values, it opens the ZED camera in live mode at RESOLUTION_HD720 and sets the depth mode to DEPTH_MODE_PERFORMANCE.\n
    You can customize it to fit your application.
    The parameters can also be saved and reloaded using its \ref save() and \ref load() functions.
     */
    struct /*@cond SHOWHIDDEN*/SL_SDK_EXPORT/*@endcond*/ InitParameters {
        friend class CameraMemberHandler;
        friend class Camera;
        ZED_SDK_VERSION_ATTRIBUTE
    public:
        /**
        Define the chosen camera resolution. Small resolutions offer higher framerate and lower computation time.\n
        In most situations, the \ref RESOLUTION "RESOLUTION_HD720" at 60 fps is the best balance between image quality and framerate.\n
        Available resolutions are listed here: \ref RESOLUTION.
        \n default : \ref RESOLUTION "RESOLUTION_HD720"
         */
        RESOLUTION camera_resolution;

        /**
        Requested camera frame rate. If set to 0, the highest FPS of the specified \ref camera_resolution will be used.\n
        See \ref RESOLUTION for a list of supported framerates.
        \n default : 0
        \note If the requested camera_fps is unsuported, the closest available FPS will be used.
         */
        int camera_fps;

        /**
        If you are using the camera upside down, setting this parameter to true will cancel its rotation. The images will be horizontally flipped.
        \n default : false
         */
        int camera_image_flip;

        /**
        At initialization, the \ref Camera runs a self-calibration process that corrects small offsets from the device's factory calibration.\n
        A drawback is that calibration parameters will sligtly change from one run to another, which can be an issue for repeatability.\n
        If set to true, self-calibration will be disabled and calibration parameters won't be optimized.\n
        default : false
        \note In most situations, self calibration should remain enabled.
        
         */
        bool camera_disable_self_calib;

        /**
        By default, the SDK only computes a single depth map, aligned with the left camera image.\n
        This parameter allows you to enable the \ref MEASURE "MEASURE_DEPTH_RIGHT" and other \ref MEASURE "MEASURE_<XXX>_RIGHT" at the cost of additional computation time.\n
        
        For example, mixed reality passthrough applications require one depth map per eye, so this parameter can be activated.
        \n default : false
         */
        bool enable_right_side_measure;

        /**
        Images coming from the camera will be saved in a FIFO buffer waiting for the program to call the \ref Camera::grab() function.
        \n <b>On Linux Desktop</b> : This parameter sets the buffer size between 2 and 5. Low values will reduce the latency but can also produce more corrupted frames.
        \n <b>On Jetson Boards</b> : This parameter is fixed to 2 for memory and performance optimizations.
        \n <b>On Windows Desktop</b> : The images aren't buffered, so this parameter won't be interpreted.

        \n default:  4 on Linux Desktop, 2 on Jetson.

        \warning Linux Desktop Only, changing this parameter has no effect on Windows or Jetson boards.
         */
        int camera_buffer_count_linux;

        /**
        <b>Only for Linux</b> : This parameter allows you to select the ZED device to be opened when multiple cameras are connected. This ID matches the system ID found in /dev/video<i>X</i>.
        \n default : 0
        \deprecated This parameters is deprecated, please check \ref input for a cross-platform and more powerful system.
         */
        int camera_linux_id;

        /**
        The \ref Camera object can be used with a live ZED or a recorded sequence saved in an SVO file.
        \n This parameter allows you to specify the path to the recorded sequence to be played back.
        \n default : (empty)
        \note If this parameter remains empty, the SDK will attempt to open a live camera. Setting it to any value will disable live mode.
        \deprecated This parameter has been deprecated with the introduction of \ref input. Please use input.setFromSVOFile("path/to/my/file.svo") instead.
         */
        String svo_input_filename;

        /**
        When playing back an SVO file, each call to \ref Camera::grab() will extract a new frame and use it.\n
        However, this ignores the real capture rate of the images saved in the SVO file.\n
        Enabling this parameter will bring the SDK closer to a real simulation when playing back a file by using the images' timestamps. However, calls to \ref Camera::grab() will return an error when trying to play to fast, and frames will be dropped when playing too slowly.
        
        \n default : false
         */
        bool svo_real_time_mode;

        /**
        The SDK offers several \ref DEPTH_MODE options offering various level of performance and accuracy.
        \n This parameter allows you to set the \ref DEPTH_MODE that best matches your needs.
        \n default : \ref DEPTH_MODE "DEPTH_MODE_PERFORMANCE"
         */
        DEPTH_MODE depth_mode;

        /**
        Regions of the generated depth map can oscillate from one frame to another. These oscillations result from a lack of texture (too homogeneous) on an object and by image noise.
        \n This parameter enables a stabilization filter that reduces these oscilations.
        \n default : true
        \note The stabilization uses the positional tracking to increase its accuracy, so the Tracking module will be enabled automatically when set to true.\n
              Notice that calling \ref Camera::enableTracking with your own parameters afterward is still possible.
         */
        int depth_stabilization;

        /**
        This parameter allows you to specify the minimum depth value (from the camera) that will be computed, measured in the \ref UNIT you define.
        \n In stereovision (the depth technology used by the camera), looking for closer depth values can have a slight impact on performance. However, this difference is almost invisible on modern GPUs.
        \n In cases of limited computation power, increasing this value can provide better performance.
        \n default : (-1) corresponding to 700 mm for a ZED and 200 mm for ZED Mini.

        \note With a ZED camera you can decrease this value to 300 mm whereas you can set it to 100 mm using a ZED Mini. In any case this value cannot be greater than 3 meters.
         */
        float depth_minimum_distance;

        /**
        This parameter allows you to select the unit to be used for all metric values of the SDK. (depth, point cloud, tracking, mesh, and others).
        \n default : \ref UNIT "UNIT_MILLIMETER"
         */
        UNIT coordinate_units;

        /**
        Positional tracking, point clouds and many other features require a given \ref COORDINATE_SYSTEM to be used as reference.
        This parameter allows you to select the \ref COORDINATE_SYSTEM use by the \ref Camera to return its measures.
        \n This defines the order and the direction of the axis of the coordinate system.
        \n default : \ref COORDINATE_SYSTEM "COORDINATE_SYSTEM_IMAGE"
         */
        COORDINATE_SYSTEM coordinate_system;

        /**
        By default the SDK will use the most powerful NVIDIA graphics card found.
        However, when running several applications, or using several cameras at the same time, splitting the load over available GPUs can be useful.
        This parameter allows you to select the GPU used by the \ref Camera using an ID from 0 to n-1 GPUs in your PC.
        \n default : -1
        \note A non-positive value will search for all CUDA capable devices and select the most powerful.
         */
        CUdevice sdk_gpu_id;

        /**
        This parameters allows you to enable the verbosity of the SDK to get a variety of runtime information in the console.
        When developing an application, enabling verbose mode can help you understand the current SDK behavior.
        \n However, this might not be desirable in a shipped version.
        \n default : false
        \note The verbose messages can also be exported into a log file. See \ref sdk_verbose_log_file for more.
         */
        bool sdk_verbose;

        /**
        When \ref sdk_verbose is enabled, this parameter allows you to redirect both the SDK verbose messages and your own application messages to a file.
        \n default : (empty) Should contain the path to the file to be written. A file will be created if missing.

        \note Setting this parameter to any value will redirect all std::cout calls of the entire program. This means that your own std::cout calls will be redirected to the log file.
        \note This parameter can be particularly useful for creating a log system, and with Unreal or Unity applications that don't provide a standard console output.
        \warning The log file won't be clear after successive executions of the application. This means that it can grow indefinitely if not cleared.
         */
        String sdk_verbose_log_file;

        /**
        If your application uses another CUDA-capable library, giving its CUDA context to the SDK can be useful when sharing GPU memories.
        \n This parameter allows you to set the CUDA context to be used by the SDK.
        \n Leaving this parameter empty asks the SDK to create its own context.
        \n default : (empty) 
        
        \note When creating you own CUDA context, you have to define the device you will use. Do not forget to also specify it on \ref sdk_gpu_id.
        \note <b>On Jetson</b>, you have to set the flag CU_CTX_SCHED_YIELD, during CUDA context creation.
        \note You can also let the SDK create its own context, and use \ref Camera::getCUDAContext() to use it.
         */
        CUcontext sdk_cuda_ctx;

        /**
        The SDK can handle different input types:
          - Select a camera by its ID (/dev/video<i>X</i> on Linux, and 0 to N cameras connected on Windows)
          - Select a camera by its serial number
          - Open a recorded sequence in the SVO file format
        This parameter allows you to select to desired input. It should be used like this:

        \code
        InitParameters init_params; // Set initial parameters
        init_params.sdk_verbose = True; // Enable verbose mode
        init_params.input.setFromCameraID(0); // Selects the camera with ID = 0
        \endcode

        \code
        InitParameters init_params; // Set initial parameters
        init_params.sdk_verbose = True; // Enable verbose mode
        init_params.input.setFromSerialNumber(1010); // Selects the camera with serial number = 1010
        \endcode

        \code
        InitParameters init_params; // Set initial parameters
        init_params.sdk_verbose = True; // Enable verbose mode
        init_params.input.setFromSVOFile("/path/to/file.svo"); // Selects the and SVO file to be read
        \endcode

        \n Available cameras and their ID/serial can be listed using \ref Camera::getDeviceList.
        \n Each \ref Camera will create its own memory (CPU and GPU), therefore the number of ZED used at the same time can be limited by the configuration of your computer. (GPU/CPU memory and capabilities)
        \n default : (empty)
        \n See \ref InputType for complementary information.
        */
        InputType input;

        /**
        Set the optional path where the SDK has to search for the settings files (SN<XXXX>.conf files). Those file contains the calibration of the camera.
        \n default : (empty). The SNXXX.conf file will be searched in the default directory (/usr/local/zed/settings/ for Linux or C:/ProgramData/stereolabs/settings for Windows)
        \note if a path is specified and no files has been found, the SDK will search on the default path (see default) for the *.conf file.
        \note Automatic download of conf file (through ZED Explorer or the installer) will still download the files on the default path. If you want to use another path by using this entry, make sure to copy the file in the proper location.

        \code
        InitParameters init_params; // Set initial parameters
        std::string home=getenv("HOME"); //get /home/user as string using getenv()
        std::string path= home+"/Documents/settings/"; //assuming /home/<user>/Documents/settings/SNXXXX.conf exists. Otherwise, it will be searched in /usr/local/zed/settings/
        init_params.optional_settings_path =sl::String(path.c_str());
        \endcode



        */
        String optional_settings_path;


        /**
        \brief Default constructor. All the parameters are set to their default and optimized values.
         */
        InitParameters(RESOLUTION camera_resolution_ = RESOLUTION_HD720,
                int camera_fps_ = 0,
                int camera_linux_id_ = 0,
                String svo_input_filename_ = String(),
                bool svo_real_time_mode_ = false,
                DEPTH_MODE depth_mode_ = DEPTH_MODE_PERFORMANCE,
                UNIT coordinate_units_ = UNIT_MILLIMETER,
                COORDINATE_SYSTEM coordinate_system_ = COORDINATE_SYSTEM_IMAGE,
                bool sdk_verbose_ = false,
                int sdk_gpu_id_ = -1,
                float depth_minimum_distance_ = -1.,
                bool camera_disable_self_calib_ = false,
                bool camera_image_flip_ = false,
                bool enable_right_side_measure_ = false,
                int camera_buffer_count_linux_ = 4,
                String sdk_verbose_log_file_ = String(),
                int depth_stabilization_ = 1,
                CUcontext sdk_cuda_ctx_ = CUcontext(),
                InputType input_type = InputType(),
                String opt_path_ = String())
        : camera_resolution(camera_resolution_)
        , camera_fps(camera_fps_)
        , camera_linux_id(camera_linux_id_)
        , svo_input_filename(svo_input_filename_)
        , svo_real_time_mode(svo_real_time_mode_)
        , depth_mode(depth_mode_)
        , coordinate_units(coordinate_units_)
        , coordinate_system(coordinate_system_)
        , sdk_verbose(sdk_verbose_)
        , sdk_gpu_id(sdk_gpu_id_)
        , depth_minimum_distance(depth_minimum_distance_)
        , camera_disable_self_calib(camera_disable_self_calib_)
        , camera_image_flip(camera_image_flip_)
        , enable_right_side_measure(enable_right_side_measure_)
        , camera_buffer_count_linux(camera_buffer_count_linux_)
        , sdk_verbose_log_file(sdk_verbose_log_file_)
        , depth_stabilization(depth_stabilization_)
        , sdk_cuda_ctx(sdk_cuda_ctx_)
        , input(input_type)
        , optional_settings_path(opt_path_){
        }
        
        /**
        This function saves the current set of parameters into a file to be reloaded with the \ref load() function.
        \param filename : the path to the file in which the parameters will be stored.
        \return True if file was successfully saved, otherwise false.

        \code
        InitParameters init_params; // Set initial parameters
        init_params.sdk_verbose = True; // Enable verbose mode
        init_params.input.setFromSVOFile("/path/to/file.svo"); // Selects the and SVO file to be read
        init_params.save("initParameters.conf"); // Export the parameters into a file
        \endcode

         */
        bool save(String filename);

        /**
        This function set the other parameters from the values contained in a previously \ref save() "saved" file.
        \param filename : the path to the file from which the parameters will be loaded.
        \return True if the file was successfully loaded, otherwise false.

        \code
        InitParameters init_params; // Set initial parameters
        init_params.load("initParameters.conf"); // Load the init_params from a previously exported file
        \endcode

        \note As the InitParameters files can be easilly modified manually (using a text editor) this functions allows you to test various settings without re-compiling your application.
         */
        bool load(String filename);
    };
    /** @} */

    /**
    \class RuntimeParameters.
    \ingroup Depth_group
    \brief Parameters that defines the behavior of the \ref grab.

    Default values are enabled.
    \n You can customize it to fit your application and then save it to create a preset that can be loaded for further executions.
     */
    struct /*@cond SHOWHIDDEN*/SL_SDK_EXPORT/*@endcond*/ RuntimeParameters {
        friend class CameraMemberHandler;
        friend class Camera;
        ZED_SDK_VERSION_ATTRIBUTE
    public:
        /**
        Defines the algorithm used for depth map computation, more info : \ref SENSING_MODE definition.
        \n default : \ref SENSING_MODE_STANDARD
         */
        SENSING_MODE sensing_mode;
        /**
        Provides 3D measures (point cloud and normals) in the desired reference frame (default is REFERENCE_FRAME_CAMERA)
        \n default : \ref REFERENCE_FRAME_CAMERA

        \note : replaces previous move_point_cloud_to_world_frame parameter.
         */
        REFERENCE_FRAME measure3D_reference_frame;

        /**
        Defines if the depth map should be computed.
        \n If false, only the images are available.
        \n default : true
         */
        bool enable_depth;

        /**
        Defines if the point cloud should be computed (including XYZRGBA).
        \n default : true

        \deprecated Point cloud is now enabled when depth is.
         */
        bool enable_point_cloud;

        /**
        \brief Default constructor, set all parameters to their default and optimized values.
         */
        RuntimeParameters(SENSING_MODE sensing_mode_ = SENSING_MODE_STANDARD,
                bool enable_depth_ = true, bool enable_point_cloud_ = true, REFERENCE_FRAME measure3D_reference_frame_ = REFERENCE_FRAME_CAMERA)
        : sensing_mode(sensing_mode_)
        , enable_depth(enable_depth_)
        , enable_point_cloud(enable_point_cloud_)
        , measure3D_reference_frame(measure3D_reference_frame_) {
        }

        /**
        \brief Saves the current set of parameters into a file.
        \param filename : the path to the file in which the parameters will be stored.
        \return true if the file was successfully saved, otherwise false.
         */
        bool save(String filename);

        /**
        \brief Loads the values of the parameters contained in a file.
        \param filename : the path to the file from which the parameters will be loaded.
        \return true if the file was successfully loaded, otherwise false.
         */
        bool load(String filename);
    };
    /** @} */

    /**
    \class TrackingParameters
    \ingroup PositionalTracking_group
    \brief Parameters for positional tracking initialization.

    A default constructor is enabled and set to its default parameters.
    \n You can customize it to fit your application and then save it to create a preset that can be loaded for further executions.

    \note Parameters can be user adjusted.
     */
    struct /*@cond SHOWHIDDEN*/SL_SDK_EXPORT/*@endcond*/ TrackingParameters {
        friend class CameraMemberHandler;
        friend class Camera;
        ZED_SDK_VERSION_ATTRIBUTE
    public:
        /**
        Position of the camera in the world frame when camera is started. By default it should be identity.
        \n Use this \ref Transform to place the camera frame in the world frame.
        \n default : Identity matrix

        \note The camera frame (defines the reference frame for the camera) is by default positioned at the world frame when tracking is started.
         */
        Transform initial_world_transform;

        /**
        This mode enables the camera to learn and remember its surroundings. This helps correct positional tracking drift and position different cameras relative to each other in space.
        \n default : true

        \warning : This mode requires few resources to run and greatly improves tracking accuracy. We recommend to leave it on by default.
         */
        bool enable_spatial_memory;
        
        /**
        This mode enables smooth pose correction for small drift correction.
        \n default : false
         */
        bool enable_pose_smoothing;
                
        /**
        This mode initialize the tracking aligned with the floor plane to better position the camera in space
        \n default : false
        \note : The floor plane detection is launched in the background until it is found.
        The tracking is in TRACKING_STATE_SEARCHING state.

        \warning : This features work best with the ZED-M since it needs an IMU to classify the floor.
        * The ZED needs to look at the floor during the initialization for optimum results.
        */
        bool set_floor_as_origin;

        /**
        Area localization file that describes the surroundings (previously saved).
        \n default : (empty)

        \note Loading an area file will start a searching phase during which the camera will try to position itself in the previously learned area.

        \warning : The area file describes a specific location. If you are using an area file describing a different location, the tracking function will continuously search for a position and may not find a correct one.
        \warning The '.area' file can only be used with the same depth mode (\ref MODE) as the one used during area recording.
         */
        String area_file_path;

        /**
        This setting allows you to enable or disable the IMU fusion. When set to false, only the optical odometry will be used.
        \n default : true
        \note This setting has no impact on the tracking of a ZED camera, only the ZED Mini uses a built-in IMU.
        */
        bool enable_imu_fusion;

        /**
        \brief  Default constructor, set all parameters to their default and optimized values.
         */
        TrackingParameters(Transform init_pos = Transform(), bool _enable_memory = true, bool _enable_pose_smoothing = false, String _area_path = String(), bool _set_floor_as_origin = false, bool _enable_imu_fusion = true)
        : initial_world_transform(init_pos)
        , enable_spatial_memory(_enable_memory)
        , enable_pose_smoothing(_enable_pose_smoothing)
        , area_file_path(_area_path)
        , set_floor_as_origin(_set_floor_as_origin)
        , enable_imu_fusion(_enable_imu_fusion) {
        }

        /**
        \brief Saves the current set of parameters into a file.
        \param filename : the path to the file in which the parameters will be stored.
        \return true if the file was successfully saved, otherwise false.
         */
        bool save(String filename);

        /**
        \brief Loads the values of the parameters contained in a file.
        \param filename : the path to the file from which the parameters will be loaded.
        \return true if the file was successfully loaded, otherwise false.
         */
        bool load(String filename);
    };

    /**
    \class SpatialMappingParameters
    \ingroup SpatialMapping_group
    \brief Sets the spatial mapping parameters.

    A default constructor is enabled and set to its default parameters.
    \n You can customize it to fit your application and then save it to create a preset that can be loaded for further executions.

    \note Parameters can be user adjusted.
     */
    struct /*@cond SHOWHIDDEN*/SL_SDK_EXPORT/*@endcond*/ SpatialMappingParameters {
        friend class CameraMemberHandler;
        friend class Camera;
        ZED_SDK_VERSION_ATTRIBUTE
    public:
        typedef std::pair<float, float> interval;

        /**
        \enum MAPPING_RESOLUTION
        \ingroup SpatialMapping_group
        \brief List the spatial mapping resolution presets.
         */
        enum MAPPING_RESOLUTION {
            MAPPING_RESOLUTION_HIGH, /**< Create a detail geometry, requires lots of memory.*/
            MAPPING_RESOLUTION_MEDIUM, /**< Smalls variations in the geometry will disappear, useful for big object*/
            MAPPING_RESOLUTION_LOW /**< Keeps only huge variations of the geometry , useful outdoor.*/
        };

        /**
        \enum MAPPING_RANGE
        \ingroup SpatialMapping_group
        \brief List the spatial mapping depth range presets.

        \deprecated Since SDK 2.6 range is computed from the requested resolution and inner parameters to best fit the current application
         */
        enum MAPPING_RANGE {
            MAPPING_RANGE_NEAR, /**< Only depth close to the camera will be used during spatial mapping.*/
            MAPPING_RANGE_MEDIUM, /**< Medium depth range.*/
            MAPPING_RANGE_FAR, /**< Takes into account objects that are far, useful outdoor.*/
            MAPPING_RANGE_AUTO  /**< Depth range will be computed based on current Camera states and parameters.*/
        };

        /**
        \brief Default constructor, set all parameters to their default and optimized values.
         */
        SpatialMappingParameters(MAPPING_RESOLUTION resolution = MAPPING_RESOLUTION_MEDIUM,
            MAPPING_RANGE range = MAPPING_RANGE_AUTO,
            int max_memory_usage_ = 2048,
            bool save_texture_ = false,
            bool use_chunk_only_ = false,
            bool reverse_vertex_order_ = false);

        /**
        \brief Return the resolution corresponding to the given \ref RESOLUTION preset.
        \param resolution : the desired \ref RESOLUTION. default : \ref RESOLUTION_HIGH.
        \return The resolution in meter.
         */
        static float get(MAPPING_RESOLUTION mapping_resolution = MAPPING_RESOLUTION_MEDIUM);
       
        /**
        \brief Sets the resolution corresponding to the given \ref RESOLUTION preset.
        \param resolution : the desired \ref RESOLUTION.  default :\ref  RESOLUTION_HIGH.
         */
        void set(MAPPING_RESOLUTION mapping_resolution = MAPPING_RESOLUTION_MEDIUM);
        
        /**
        \brief  Return the recommanded maximum depth value for the given \ref MAPPING_RESOLUTION preset.
        \param mapping_resolution : the desired \ref MAPPING_RESOLUTION.
        \param camera : the Camera object which will run the spatial mapping.
        \return The maximum value of depth in meters.
        */
        static float getRecommendedRange(MAPPING_RESOLUTION mapping_resolution, Camera &camera);

        /**
        \brief  Return the recommanded maximum depth value for the given \ref MAPPING_RESOLUTION preset.
        \param resolution : the desired resolution in meters.
        \param camera : the Camera object which will run the spatial mapping.
        \return The maximum value of depth in meters.
        */
        static float getRecommendedRange(float resolution_meters, Camera &camera);

        /**
        \brief  Return the maximum value of depth corresponding to the given \ref RANGE presets.
        \param range : the desired \ref RANGE. default : \ref RANGE_MEDIUM.
        \return The maximum value of depth.
        
        \deprecated Since SDK 2.6 range is computed from the requested resolution and inner parameters to best fit the current application        
         */
        /*@cond SHOWHIDDEN*/SL_DEPRECATED(/*@endcond*/static float get(MAPPING_RANGE mapping_range = MAPPING_RANGE_MEDIUM)/*@cond SHOWHIDDEN*/)/*@endcond*/;

        /**
        \brief Sets the maximum value of depth corresponding to the given \ref RANGE presets.
        \param range : the desired \ref RANGE. default : \ref RANGE_MEDIUM.
        
        \deprecated Since SDK 2.6 range is computed from the requested resolution and inner parameters to best fit the current application
         */
        /*@cond SHOWHIDDEN*/SL_DEPRECATED(/*@endcond*/ void set(MAPPING_RANGE mapping_range = MAPPING_RANGE_MEDIUM)/*@cond SHOWHIDDEN*/)/*@endcond*/;
        
        /**
        \brief Spatial mapping resolution in meters, should fit \ref allowed_resolution.
         */
        float resolution_meter = 0.05f;

        /**
        \brief The resolutions allowed by the spatial mapping.
        \n allowed_resolution.first is the minimum value allowed.
        \n allowed_resolution.seconf is the maximum value allowed.
         */
        const interval allowed_resolution = std::make_pair(0.01f, 0.2f);

        /**
        \brief Depth range in meters.
        Can be different from the value set by \ref setDepthMaxRangeValue.
        \n Is sets to 0 by default, in this case the range is computed from resolution_meter 
        and from the currents inner parameters to fit your application.
        
        \deprecated Since SDK 2.6 range is computed from the requested resolution and inner parameters to best fit the current application
         */
        float range_meter = 0.f;

        /**
        \brief Range of the maximal depth value allowed by the spatial mapping.
        \n allowed_range.first is the minimum value allowed.
        \n allowed_range.seconf is the maximum value allowed.
         */
        const interval allowed_range = std::make_pair(2.f, 20.f);

        /**
        \brief Set to true if you want to be able to apply the texture to your mesh after its creation.

        \note This option will take more memory.
         */
        bool save_texture = false;

        /**
        \brief Set to false if you want to keep consistency between the mesh and its inner chunks data.

        \note Updating the Mesh is time consuming, consider using only Chunks data for better performances.
         */
        bool use_chunk_only = false;

        /**
        \brief The maximum CPU memory (in mega bytes) allocated for the meshing process.
         */
        int max_memory_usage = 2048;

        /**
        \brief Specify if the order of the vertices of the triangles needs to be inverted. If your display process does not handle front and back face culling you can use this to set it right.
         */
        bool reverse_vertex_order = false;

        /**
        \brief Saves the current set of parameters into a file.
        \param filename : the path to the file in which the parameters will be stored.
        \return true if the file was successfully saved, otherwise false.
         */
        bool save(String filename);

        /**
        \brief Loads the values of the parameters contained in a file.
        \param filename : the path to the file from which the parameters will be loaded.
        \return true if the file was successfully loaded, otherwise false.
         */
        bool load(String filename);
    };
    class CameraMemberHandler;

    /**
    \class Camera
    \ingroup Video_group
    \brief This class is the main interface with the camera and the SDK features, suche as: video, depth, tracking, mapping, and more.
    \n Find more information in the detailed description below.\n

     
     A standard program will use the \ref Camera class like this:
     \code
     #include <sl/Camera.hpp>

     using namespace sl;

     int main(int argc, char **argv) {

         // --- Initialize a Camera object and open the ZED
         // Create a ZED camera object
         Camera zed;

         // Set configuration parameters
         InitParameters init_params;
         init_params.camera_resolution = RESOLUTION_HD720; // Use HD720 video mode
         init_params.camera_fps = 60; // Set fps at 60

         // Open the camera
         ERROR_CODE err = zed.open(init_params);
         if (err != SUCCESS) {
             std::cout << toString(err) << std::endl;
             exit(-1);
         }

         sl::RuntimeParameters runtime_param;
         runtime_param.sensing_mode = SENSING_MODE_STANDARD;

         // --- Main loop grabing images and depth values
         // Capture 50 frames and stop
         int i = 0;
         Mat image, depth;
         while (i < 50) {
             // Grab an image
             if (zed.grab(runtime_param) == SUCCESS) { // A new image is available if grab() returns SUCCESS

                 //Display a pixel color
                 zed.retrieveImage(image, VIEW_LEFT); // Get the left image
                 sl::uchar4 centerRGB;
                 image.getValue<sl::uchar4>(image.getWidth() / 2, image.getHeight() / 2, &centerRGB);
                 std::cout << "Image " << i << " center pixel R:" << (int)centerRGB.r << " G:" << (int)centerRGB.g << " B:" << (int)centerRGB.b << std::endl;

                 //Display a pixel depth
                 zed.retrieveMeasure(depth, MEASURE_DEPTH); // Get the depth map
                 float centerDepth;
                 depth.getValue<float>(depth.getWidth() / 2, depth.getHeight() / 2, &centerDepth);
                 std::cout << "Image " << i << " center depth:" << centerDepth << std::endl;

                 i++;
             }
         }

         // --- Close the Camera
         zed.close();
         return 0;
     }
     \endcode
     *
     */
    class /*@cond SHOWHIDDEN*/SL_SDK_EXPORT/*@endcond*/ Camera {
        friend CameraMemberHandler;
        friend SpatialMappingParameters;
        ZED_SDK_VERSION_ATTRIBUTE

    public:
        /**
        \brief Default constructor which creates an empty Camera object.
        \n Parameters will be set when calling \ref open() "open(init_param)" with the desired \ref InitParameters .

        The Camera object can be created like this:
        \code
        Camera zed;
        \endcode
        or
        \code
        Camera* zed = new Camera();
        \endcode
         */
        Camera();

        /**
        \brief The Camera destructor will call the \ref close() function and clear the memory previously allocated by the object.
         */
        ~Camera();

        /**
        \brief Opens the ZED camera from the provided \ref InitParameter.\n
        This function will also check the hardware requirements and run a self-calibration.

        \param init_parameters : a structure containing all the initial parameters. default : a preset of InitParameters.
        \return An error code giving information about the internal process. If \ref ERROR_CODE "SUCCESS" is returned, the camera is ready to use. Every other code indicates an error and the program should be stopped.

        Here is the proper way to call this function:
        \code
        Camera zed; // Create a ZED camera object

        InitParameters init_params; // Set configuration parameters
        init_params.camera_resolution = RESOLUTION_HD720; // Use HD720 video mode
        init_params.camera_fps = 60; // Set fps at 60

        // Open the camera
        ERROR_CODE err = zed.open(init_params);
        if (err != SUCCESS) {
            std::cout << toString(err) << std::endl; // Display the error
            exit(-1);
        }
        \endcode

        \note If you are having issues opening a camera, the diagnostic tool provided in the SDK can help you identify to problems.
        \n If this function is called on an already opened camera, \ref close() will be called.
        \n <b>Windows:</b> C:\\Program Files (x86)\\ZED SDK\\tools\\ZED Diagnostic.exe
        \n <b>Linux:</b> /usr/local/zed/tools/ZED Diagnostic
         */
        ERROR_CODE open(InitParameters init_parameters = InitParameters());

        /**
        \brief Reports if the camera has been successfully opened. It has the same behavior as checking if \ref open() returns \ref ERROR_CODE "SUCCESS".
        \return true if the ZED is already setup, otherwise false.
         */
        inline bool isOpened() {
            return opened;
        }

        /**
        \brief If \ref open() has been called, this function will close the connection to the camera (or the SVO file) and free the corresponding memory.

        If \ref open() wasn't called or failed, this function won't have any effects.
        \note If an asynchronous task is running within the \ref Camera object, like \ref saveCurrentArea(), this function will wait for its completion.
        \n The \ref open() function can then be called if needed.
        
        \warning If the CUDA context was created by \ref open(), this function will destroy it. Please make sure to delete your GPU \ref sl::Mat objects before the context is destroyed.
         */
        void close();


        /**
        \brief This function will grab the latest images from the camera, rectify them, and compute the \ref retrieveMeasure() "measurements" based on the \ref RuntimeParameters provided (depth, point cloud, tracking, etc.)
        \n As measures are created in this function, its execution can last a few milliseconds, depending on your parameters and your hardware.
        \n The exact duration will mostly depend on the following parameters:
           - \ref InitParameters.enable_right_side_measure : Activating this parameter increases computation time
           - \ref InitParameters.depth_stabilization : Stabilizing the depth requires an additional computation load as it enables tracking
           - \ref InitParameters.camera_resolution : Lower resolutions are faster to compute
           - \ref InitParameters.depth_mode : \ref DEPTH_MODE "PERFORMANCE" will run faster than \ref DEPTH_MODE "ULTRA"
           - \ref enableTracking() : Activating the tracking is an additional load
           - \ref RuntimeParameters.sensing_mode : \ref SENSING_MODE "STANDARD" mode will run faster than \ref SENSING_MODE "FILL" mode, which needs to estimate the depth of occluded pixels.
           - \ref RuntimeParameters.enable_depth : Avoiding the depth computation must be faster. However, it is required by most SDK features (tracking, spatial mapping, plane estimation, etc.)

        If no images are available yet, \ref ERROR_CODE "ERROR_CODE_NOT_A_NEW_FRAME" will be returned.
        \n This function is meant to be called frequently in the main loop of your application.

        \param rt_parameters : a structure containing all the runtime parameters. default : a preset of \ref RuntimeParameters.
        \return Returning \ref ERROR_CODE "SUCCESS" means that no problem was encountered. Returned errors can be displayed using \ref toString(error)

        \code
        // Set runtime parameters after opening the camera
        RuntimeParameters runtime_param;
        runtime_param.sensing_mode = SENSING_MODE_STANDARD; // Use STANDARD sensing mode

        Mat image;
        while (true) {
            // Grab an image
            if (zed.grab(runtime_param) == SUCCESS) { // A new image is available if grab() returns SUCCESS
                zed.retrieveImage(image, VIEW_LEFT); // Get the left image
                
                // Use the image for your application
            }
        }
        \endcode
         */
        ERROR_CODE grab(RuntimeParameters rt_parameters = RuntimeParameters());

        /**
        \brief Returns the calibration parameters, serial number and other information about the camera being used.

        As calibration parameters depend on the image resolution, you can provide a custom resolution as a parameter to get scaled information.
        \n When reading an SVO file, the parameters will correspond to the camera used for recording.

        \param image_size : You can specify a size different from default image size to get the scaled camera information. default = (0,0) meaning original image size (aka \ref getResolution() ).
        \return \ref CameraInformation containing the calibration parameters of the ZED, as well as serial number and firmware version.

        \note The returned parameters might vary between two execution due to the \ref InitParameters.camera_disable_self_calib "self-calibration" being ran in the \ref open() method.
         */
        CameraInformation getCameraInformation(Resolution image_size = Resolution(0, 0));

        /**
        \brief Gets the Camera-created CUDA context for sharing it with other CUDA-capable libraries. This can be useful for sharing GPU memories.

        If you're looking for the opposite mechanism, where an existing CUDA context is given to the \ref Camera, please check \ref InitParameters.sdk_cuda_ctx

        \return The CUDA context used for GPU calls.
         */
        CUcontext getCUDAContext();

        ///@{
        /// @name Video Functions
        // -----------------------------------------------------------------
        //                         Video :
        // -----------------------------------------------------------------

        /**
        \brief Returns the size of the grabbed images.

        In live mode it matches \ref InitParameters.camera_resolution.
        \n In SVO mode the recording resolution will be returned.
        \n All the <b>default</b> \ref retrieveImage() and \ref retrieveMeasure() calls will generate an image matching this resolution.
        \return The grabbed images resolution.
         */
        Resolution getResolution();

        /**
        \brief Retrieves images from the camera (or SVO file).

        Multiple images are available along with a view of various measures for display purposes.
        \n Available images and views are listed \ref VIEW "here".
        \n As an example, \ref VIEW "VIEW_DEPTH" can be used to get a grayscale version of the depth map, but the actual depth values can be retrieved using \ref retrieveMeasure().
        \n
        \n <b>Memory</b>
        \n By default, images are copied from GPU memory to CPU memory (RAM) when this function is called.
        \n If your application can use GPU images, using the <b>type</b> parameter can increase performance by avoiding this copy.
        \n If the provided \ref Mat object is already allocated  and matches the requested image format, memory won't be re-allocated.
        \n
        \n <b>Image size</b>
        \n By default, images are returned in the resolution provided by \ref getResolution().
        \n However, you can request custom resolutions. For example, requesting a smaller image can help you speed up your application.

        \param mat : \b [out] the \ref Mat to store the image.
        \param view  : defines the image you want (see \ref VIEW). default : \ref VIEW "VIEW_LEFT".
        \param type : whether the image should be provided in CPU or GPU memory. default : \ref MEM "MEM_CPU."
        \param width : if specified, define the width of the output mat. If set to 0, the width of the ZED resolution will be taken. default : 0.
        \param height : if specified, define the height of the output mat. If set to 0, the height of the ZED resolution will be taken. default : 0.
        \return \ref "SUCCESS" if the method succeeded, \ref ERROR_CODE "ERROR_CODE_FAILURE" if an error occurred.

        \note As this function retrieves the images grabbed by the \ref grab() function, it should be called afterward.

        \code
        Mat leftImage, depthView; //create sl::Mat objects to store the images
        while (true) {
        // Grab an image
            if (zed.grab() == SUCCESS) { // A new image is available if grab() returns SUCCESS
                zed.retrieveImage(leftImage, VIEW_LEFT); // Get the rectified left image
                zed.retrieveImage(depthView, VIEW_DEPTH); // Get a grayscale preview of the depth map

                //Display the center pixel colors
                sl::uchar4 leftCenter;
                leftImage.getValue<sl::uchar4>(leftImage.getWidth() / 2, leftImage.getHeight() / 2, &leftCenter);
                std::cout << "leftImage center pixel R:" << (int)leftCenter.r << " G:" << (int)leftCenter.g << " B:" << (int)leftCenter.b << std::endl;

                sl::uchar4 depthCenter;
                depthView.getValue<sl::uchar4>(depthView.getWidth() / 2, depthView.getHeight() / 2, &depthCenter);
                std::cout << "depthView center pixel R:" << (int)depthCenter.r << " G:" << (int)depthCenter.g << " B:" << (int)depthCenter.b << std::endl;
            }
        }
        \endcode
         */
        ERROR_CODE retrieveImage(Mat &mat, VIEW view = VIEW_LEFT, MEM type = MEM_CPU, int width = 0, int height = 0);

        /**
        \brief Returns the current value of the requested \ref CAMERA_SETTINGS "camera setting". (gain, brightness, hue, exposure, etc.)

        Possible values (range) of each setting are available \ref CAMERA_SETTINGS "here".

        \param setting : the requested setting.
        \return The current value for the corresponding setting. Returns -1 if encounters an error.

        \code
        int gain = zed.getCameraSettings(CAMERA_SETTINGS_GAIN);
        std::cout << "Current gain value: " << gain << std::endl;
        \endcode

        \note Works only if the camera is open in live mode. (Settings aren't exported in the SVO file format)
         */
        int getCameraSettings(CAMERA_SETTINGS settings);

        /**
        \brief Sets the value of the requested \ref CAMERA_SETTINGS "camera setting". (gain, brightness, hue, exposure, etc.)

        Possible values (range) of each setting are available \ref CAMERA_SETTINGS "here".

        \param settings : the setting to be set.
        \param value : the value to set.
        \param use_default : will set default (or automatic) value if set to true. If so, <b>Value</b> parameter will be ignored. default: false.

        \code
        //Set the gain to 50
        zed.setCameraSettings(CAMERA_SETTINGS_GAIN, 50, false);
        \endcode

        \warning Setting \ref CAMERA_SETTINGS_EXPOSURE or \ref CAMERA_SETTINGS_GAIN to default will automatically sets the other to default.

        \note Works only if the camera is open in live mode.
         */
        void setCameraSettings(CAMERA_SETTINGS settings, int value, bool use_default = false);

        /**
        \brief Returns the framerate of the camera.

        In live mode, this value should match \ref InitParameters.camera_fps.
        \n When playing an SVO file, this value matches the requested framerate of the recording camera.

        \return The framerate at wich the ZED is streaming its images (or the corresponding recorded value in SVO mode). Returns -1.f if it encounters an error.

        \warning The actual framerate (number of images retrieved per second) can be lower if the \ref grab() function runs slower than the image stream or is called too often.
        
        \code
        int camera_fps = zed.getCameraFPS();
        std::cout << "Camera framerate: " << camera_fps << std::endl;
        \endcode

        \see getCurrentFPS()
         */
        float getCameraFPS();

        /**
        \brief Sets a new target frame rate for the camera.
        
        When a live camera is opened, this function allows you to override the value previously set in \ref InitParameters.camera_fps.
        \n It has no effect when playing back an SVO file.
        \param desired_fps : the new desired frame rate.
        \deprecated This function is no more supported and can cause stability issues.
        \warning This function isn't thread safe and will be removed in a later version.
        \n If you want to artificially reduce the camera's framerate, you can lower the frequency at which you call the \ref grab() method.
        \n If a not-supported framerate is requested, the closest available setting will be used.
         * 
        \note Works only if the camera is open in live mode.
         */
        /*@cond SHOWHIDDEN*/SL_DEPRECATED(/*@endcond*/void setCameraFPS(int desired_fps)/*@cond SHOWHIDDEN*/)/*@endcond*/;

        /**
        \brief Returns the current framerate at which the \ref grab() method is successfully called.

        The returned value is based on the difference of camera \ref getTimestamp() "timestamps" between two successful grab() calls.

        \return The current SDK framerate

        \warning The returned framerate (number of images grabbed per second) can be lower than \ref InitParameters.camera_fps if the \ref grab() function runs slower than the image stream or is called too often.

        \code
        int current_fps = zed.getCurrentFPS();
        std::cout << "Current framerate: " << current_fps << std::endl;
        \endcode
         */
        float getCurrentFPS();


        /**
        \brief Returns the timestamp in the requested \ref TIME_REFERENCE.

        - When requesting the \ref TIME_REFERENCE "TIME_REFERENCE_IMAGE" timestamp, the UNIX nanosecond timestamp of the latest \ref grab() "grabbed" image will be returned.
        \n This value corresponds to the time at which the entire image was available in the PC memory. As such, it ignores the communication time that corresponds to 2 or 3 frame-time based on the fps (ex: 33.3ms to 50ms at 60fps).

        - When requesting the \ref TIME_REFERENCE "TIME_REFERENCE_CURRENT" timestamp, the current UNIX nanosecond timestamp is returned.
        
        \n This function can also be used when playing back an SVO file.

        \param reference_time : The selected \ref TIME_REFERENCE.

        \return The timestamp in nanosecond. 0 if not available (SVO file without compression).

        \note As this function returns UNIX timestamps, the reference it uses is common accros several \ref Camera instances.
        \n This can help to organized the grabbed images in a multi-camera application.

        \code
        timeStamp last_image_timestamp = zed.getTimestamp(TIME_REFERENCE_IMAGE);
        timeStamp current_timestamp = zed.getTimestamp(TIME_REFERENCE_CURRENT);

        std::cout << "Latest image timestamp: " << last_image_timestamp << "ns from Epoch." << std::endl;
        std::cout << "Current timestamp: " << current_timestamp << "ns from Epoch." << std::endl;
        \endcode
         */
        timeStamp getTimestamp(sl::TIME_REFERENCE reference_time);


        /**
        \brief This function has been deprecated. Please refer to \ref getTimestamp "getTimestamp(TIME_REFERENCE_IMAGE)" which has the exact same behavior.
       
        \deprecated See \ref getTimestamp() with \ref TIME_REFERENCE "TIME_REFERENCE_IMAGE".
         */
        /*@cond SHOWHIDDEN*/SL_DEPRECATED/*@endcond*/(timeStamp getCameraTimestamp()/*@cond SHOWHIDDEN*/)/*@endcond*/;


        /**
        \brief This function has been deprecated. Please refer to \ref getTimestamp "getTimestamp(TIME_REFERENCE_CURRENT)" which has the exact same behavior.
       
        \deprecated See \ref getTimestamp() with \ref TIME_REFERENCE "TIME_REFERENCE_CURRENT".
         */
        /*@cond SHOWHIDDEN*/SL_DEPRECATED/*@endcond*/(timeStamp getCurrentTimestamp()/*@cond SHOWHIDDEN*/)/*@endcond*/;


        /**
        \brief Returns the number of frames dropped since \ref grab() was called for the first time.

        A dropped frame corresponds to a frame that never made it to the grab function.
        \n This can happen if two frames were extracted from the camera when grab() is called. The older frame will be dropped so as to always use the latest (which minimizes latency).
        
        \return The number of frames dropped since the first \ref grab() call.
         */
        unsigned int getFrameDroppedCount();

        /**
        \brief Returns the current playback position in the SVO file.

        The position corresponds to the number of frames already read from the SVO file, starting from 0 to n.
        \n Each \ref grab() call increases this value by one (except when using \ref InitParameters.svo_real_time_mode).

        \return The current frame position in the SVO file. Returns -1 if the SDK is not reading an SVO.

        \note Works only if the camera is open in SVO playback mode.
        \see setSVOPosition() for an example.
         */
        int getSVOPosition();

        /**
        \brief Sets the playback cursor to the desired frame number in the SVO file.

        This function allows you to move around within a played-back SVO file. After calling, the next call to \ref grab() will read the provided frame number.

        \param frame_number : the number of the desired frame to be decoded.

        \note Works only if the camera is open in SVO playback mode.

        \code
        #include <sl/Camera.hpp>

        using namespace sl;

        int main(int argc, char **argv) {

            // Create a ZED camera object
            Camera zed;

            // Set configuration parameters
            InitParameters init_params;
            init_params.input.setFromSVOFile("path/to/my/file.svo");

            // Open the camera
            ERROR_CODE err = zed.open(init_params);
            if (err != SUCCESS) {
                std::cout << toString(err) << std::endl;
                exit(-1);
            }

            // Loop between frame 0 and 50
            int i = 0;
            Mat leftImage;
            while (zed.getSVOPosition() < zed.getSVONumberOfFrames()-1) {

                std::cout << "Current frame: " << zed.getSVOPosition() << std::endl;

                // Loop if we reached frame 50
                if (zed.getSVOPosition() == 50)
                    zed.setSVOPosition(0);

                // Grab an image
                if (zed.grab() == SUCCESS) {
                    zed.retrieveImage(leftImage, VIEW_LEFT); // Get the rectified left image

                    // Use the image in your application
                }
            }

            // Close the Camera
            zed.close();
            return 0;
        }
        \endcode
         */
        void setSVOPosition(int frame_number);

        /**
        \brief Returns the number of frames in the SVO file.
        \return The total number of frames in the SVO file (-1 if the SDK is not reading a SVO).

        \note Works only if the camera is open in SVO reading mode.
        \see setSVOPosition() for an example.
         */
        int getSVONumberOfFrames();

        /**
        \brief Returns the current status of the self-calibration.

        When opening the camera, the ZED will self-calibrate itself to optimize the factory calibration.
        \n As this process can run slightly slower than \ref open(), this function allows you to check its status.
        \n The self-calibration can be disabled using \ref InitParameters.camera_disable_self_calib .

        \return A status code giving information about the status of the self calibration.
        \note The first call to the \ref grab() function will wait for the self-calibration to finish.
        \see SELF_CALIBRATION_STATE
         */
        SELF_CALIBRATION_STATE getSelfCalibrationState();

        /**
        \brief Resets the camera's self calibration. This function can be called at any time <b>after</b> the \ref open() function.

        It will reset and optimize the calibration parameters against misalignment, convergence, and color mismatch.
        It can be called if the calibration file of the current camera has been updated while the application is running.

        If the self-calibration didn't succeed, previous parameters will be used.
        \note The next call to the \ref grab() function will wait for the self-calibration to finish.
         */
        void resetSelfCalibration();
        ///@}

        ///@{
        /// @name Depth Sensing Functions
        // -----------------------------------------------------------------
        //                         Depth functions:
        // -----------------------------------------------------------------

        /**
        \brief Computed measures, like depth, point cloud, or normals, can be retrieved using this method.
        
        \n Multiple measures are available after a \ref grab() call. A full list is available \ref MEASURE "here".
        \n
        \n <b>Memory</b>
        \n By default, images are copied from GPU memory to CPU memory (RAM) when this function is called. 
        \n If your application can use GPU images, using the \b type parameter can increase performance by avoiding this copy. 
        \n If the provided \ref Mat object is already allocated and matches the requested image format, memory won't be re-allocated.
        \n
        \n <b>Measure size</b>
        \n By default, measures are returned in the resolution provided by \ref getResolution().
        \n However, custom resolutions can be requested. For example, requesting a smaller measure can help you speed up your application.
       

        \param mat : \b [out] the \ref Mat to store the measures.
        \param measure : defines the measure you want. (see \ref MEASURE), default : \ref MEASURE "MEASURE_DEPTH"
        \param type : the type of the memory of provided mat that should by used. default : MEM_CPU.
        \param width : if specified, define the width of the output mat. If set to 0, the width of the ZED resolution will be taken. default : 0
        \param height : if specified, define the height of the output mat. If set to 0, the height of the ZED resolution will be taken. default : 0
        \return \ref "SUCCESS" if the method succeeded, \ref ERROR_CODE "ERROR_CODE_FAILURE" if an error occurred.

        \note As this function retrieves the measures computed by the \ref grab() function, it should be called after.
        \n
        \n Measures containing "RIGHT" in their names, requires \ref InitParameters.enable_right_side_measure to be enabled.

        \code
        Mat depthMap, pointCloud;
        int x = zed.getResolution().width / 2; // Center coordinates
        int y = zed.getResolution().height / 2;

        while (true) {
            if (zed.grab() == SUCCESS) { // Grab an image

                zed.retrieveMeasure(depthMap, MEASURE_DEPTH, MEM_CPU); // Get the depth map
                // Read a depth value
                float centerDepth = 0;
                depthMap.getValue<float>(x, y, &centerDepth, MEM_CPU); // each depth map pixel is a float value
                if (isnormal(centerDepth)) { // + Inf is "too far", -Inf is "too close", Nan is "unknown/occlusion"
                    std::cout << "Depth value at center: " << centerDepth << " " << init_params.coordinate_units << std::endl;
                }


                zed.retrieveMeasure(pointCloud, MEASURE_XYZRGBA, MEM_CPU);// Get the point cloud
                // Read a point cloud value
                sl::float4 pcValue;
                pointCloud.getValue<sl::float4>(x, y, &pcValue); // each point cloud pixel contains 4 floats, so we are using a sl::float4
                if (isnormal(pcValue.z)) {
                    std::cout << "Point cloud coordinates at center: X=" << pcValue.x << ", Y=" << pcValue.y << ", Z=" << pcValue.z << std::endl;
                    unsigned char color[sizeof(float)];
                    memcpy(color, &pcValue[3], sizeof(float));
                    std::cout << "Point cloud color at center: R=" << (int)color[0] << ", G=" << (int)color[1] << ", B=" << (int)color[2] << std::endl;
                }
            }
        }
        \endcode
         */
        ERROR_CODE retrieveMeasure(Mat &mat, MEASURE measure = MEASURE_DEPTH, MEM type = MEM_CPU, int width = 0, int height = 0);

        /**
        \brief Returns the current maximum distance of depth estimation.

        When estimating the depth, the SDK uses this upper limit to turn higher values into \ref TOO_FAR ones.

        \return The current maximum distance that can be computed in the defined \ref UNIT.
        \note This method doesn't return the highest value of the current depth map, but the highest possible one.
         */
        float getDepthMaxRangeValue();

        /**
        \brief Sets the maximum distance of depth estimation (All values beyond this limit will be reported as \ref TOO_FAR).

        This method can be used to extend or reduce the depth perception range. However, the depth accuracy decreases with distance.

        \param depth_max_range : maximum distance in the defined \ref UNIT.
        \note Changing this value has no impact on performance and doesn't affect the positional tracking nor the spatial mapping. (Only the depth, point cloud, normals)
         */
        void setDepthMaxRangeValue(float depth_max_range);

        /**
        \brief Returns the closest measurable distance by the camera, according to the camera and the depth map parameters.

        When estimating the depth, the SDK uses this lower limit to turn lower values into \ref TOO_CLOSE ones.

        \return The minimum distance that can be computed in the defined \ref UNIT.
        \note This method doesn't return the lowest value of the current depth map, but the lowest possible one.
         */
        float getDepthMinRangeValue();

        /**
        \brief Returns the current confidence threshold value applied to the depth map.

        Each depth pixel has a corresponding confidence. (\ref MEASURE "MEASURE_CONFIDENCE")
        \n This function returns the value currently used to reject unconfident depth pixels.
        \n By default, the confidence threshold is set at 100, meaning that no depth pixel will be rejected.

        \return The current threshold value between 0 and 100.
        \see setConfidenceThreshold()
         */
        int getConfidenceThreshold();

        /**
        \brief Sets a threshold to reject depth values based on their confidence.

        Each depth pixel has a corresponding confidence. (\ref MEASURE "MEASURE_CONFIDENCE")
        \n A lower value means more confidence and precision (but less density). An upper value reduces filtering (more density, less certainty).
        \n - \b setConfidenceThreshold(100) will allow values from \b 0 to \b 100. (no filtering)
        \n - \b setConfidenceThreshold(90) will allow values from \b 10 to \b 100. (filtering lowest confidence values)
        \n - \b setConfidenceThreshold(30) will allow values from \b 70 to \b 100. (keeping highest confidence values and lowering the density of the depth map)

        \param conf_threshold_value : a value in [1,100].
         */
        void setConfidenceThreshold(int conf_threshold_value);
        ///@}

        ///@{
        /// @name Positional Tracking Functions
        // -----------------------------------------------------------------
        //                        Positional Tracking functions:
        // -----------------------------------------------------------------

        /**
        \brief Initializes and starts the positional tracking processes.

        This function allows you to enable the position estimation of the SDK. It only has to be called once in the camera's lifetime.
        \n When enabled, the \ref getPosition "position" will be update at each grab call.
        \n Tracking-specific parameter can be set by providing \ref TrackingParameters to this function.

        \param tracking_parameters : A structure containing all the \ref TrackingParameters . default : a preset of \ref TrackingParameters.
        \return \ref ERROR_CODE "ERROR_CODE_FAILURE" if the \ref area_file_path file wasn't found, \ref ERROR_CODE "SUCCESS" otherwise.
        \warning The positional tracking feature benefits from a high framerate. We found HD720@60fps to be the best compromise between image quality and framerate.

        \code
        #include <sl/Camera.hpp>

        using namespace sl;

        int main(int argc, char **argv) {

            // --- Initialize a Camera object and open the ZED
            // Create a ZED camera object
            Camera zed;

            // Set configuration parameters
            InitParameters init_params;
            init_params.camera_resolution = RESOLUTION_HD720; // Use HD720 video mode
            init_params.camera_fps = 60; // Set fps at 60

            // Open the camera
            ERROR_CODE err = zed.open(init_params);
            if (err != SUCCESS) {
                std::cout << toString(err) << std::endl;
                exit(-1);
            }

            // Set tracking parameters
            TrackingParameters track_params;
            track_params.enable_spatial_memory = true;

            // Enable positional tracking
            err = zed.enableTracking(track_params);
            if (err != SUCCESS) {
                std::cout << "Tracking error: " << toString(err) << std::endl;
                exit(-1);
            }

            // --- Main loop
            while (true) {
                if (zed.grab() == SUCCESS) { // Grab an image and computes the tracking
                    Pose cameraPose;
                    zed.getPosition(cameraPose, REFERENCE_FRAME_WORLD);
                    std::cout << "Camera position: X=" << cameraPose.getTranslation().x << " Y=" << cameraPose.getTranslation().y << " Z=" << cameraPose.getTranslation().z << std::endl;
                }
            }

            // --- Close the Camera
            zed.close();
            return 0;
        }
        \endcode
         */
        ERROR_CODE enableTracking(TrackingParameters tracking_parameters = TrackingParameters());

        /**
        \brief Retrieves the estimated position and orientation of the camera in the specified \ref REFERENCE_FRAME "reference frame".

        \n Using \ref REFERENCE_FRAME "REFERENCE_FRAME_WORLD", the returned pose relates to the initial position of the camera. (\ref TrackingParameters.initial_world_transform )
        \n Using \ref REFERENCE_FRAME "REFERENCE_FRAME_CAMERA", the returned pose relates to the previous position of the camera.
        \n
        \n If the tracking has been initialized with \ref TrackingParameters.enable_spatial_memory to true (default), this function can return \ref TRACKING_STATE "TRACKING_STATE_SEARCHING".
        \n This means that the tracking lost its link to the initial referential and is currently trying to relocate the camera. However, it will keep on providing position estimations.

        \param camera_pose \b [out]: the pose containing the position of the camera and other information (timestamp, confidence)
        \param reference_frame : defines the reference from which you want the pose to be expressed. Default : \ref REFERENCE_FRAME "REFERENCE_FRAME_WORLD".
        \return The current \ref TRACKING_STATE "state" of the tracking process.

        \n Extract Rotation Matrix : camera_pose.getRotation();
        \n Extract Translation Vector: camera_pose.getTranslation();
        \n Convert to Orientation / quaternion : camera_pose.getOrientation();

        \note The position is provided in the \ref InitParameters.coordinate_system . See \ref COORDINATE_SYSTEM for its physical origin.
        \warning This function requires the tracking to be enabled. \ref enableTracking() .

        \code
        // --- Main loop
        while (true) {
            if (zed.grab() == SUCCESS) { // Grab an image and computes the tracking
                Pose cameraPose;
                zed.getPosition(cameraPose, REFERENCE_FRAME_WORLD);
                std::cout << "Camera position: X=" << cameraPose.getTranslation().x << " Y=" << cameraPose.getTranslation().y << " Z=" << cameraPose.getTranslation().z << std::endl;
                std::cout << "Camera Euler rotation: X=" << cameraPose.getEulerAngles().x << " Y=" << cameraPose.getEulerAngles().y << " Z=" << cameraPose.getEulerAngles().z << std::endl;
                std::cout << "Camera Rodrigues rotation: X=" << cameraPose.getRotationVector().x << " Y=" << cameraPose.getRotationVector().y << " Z=" << cameraPose.getRotationVector().z << std::endl;
                std::cout << "Camera quaternion orientation: X=" << cameraPose.getOrientation().x << " Y=" << cameraPose.getOrientation().y << " Z=" << cameraPose.getOrientation().z << " W=" << cameraPose.getOrientation().w << std::endl;
                std::cout << std::endl;
            }
        }
        \endcode
         */
        TRACKING_STATE getPosition(Pose &camera_pose, REFERENCE_FRAME reference_frame = REFERENCE_FRAME_WORLD);

        /**
        \brief Saves the current area learning file. The file will contain spatial memory data generated by the tracking.

        If the tracking has been initialized with \ref TrackingParameters.enable_spatial_memory to true (default), the function allows you to export the spatial memory.
        \n Reloading the exported file in a future session with \ref TrackingParameters.area_file_path initialize the tracking within the same referential.
        \n This function is asynchronous, and only triggers the file generation. You can use \ref getAreaExportState() to get the export state.
        The positional tracking keeps running while exporting.

        \param area_file_path : save the spatial memory database in an '.area' file.
        \return \ref ERROR_CODE "ERROR_CODE_FAILURE" if the \ref area_file_path file wasn't found, \ref SUCCESS otherwise.
        \see getAreaExportState()

        \note Please note that this function will also flush the area database that was built / loaded.
        \warning If the camera wasn't moved during the tracking session, or not enough, the spatial memory won't be usable and the file won't be exported.
        \n The \ref getAreaExportState() function will return \ref AREA_EXPORT_STATE "AREA_EXPORT_STATE_NOT_STARTED".
        \n A few meters (~3m) of translation or a full rotation should be enough to get usable spatial memory.
        \n However, as it should be used for relocation purposes, visiting a significant portion of the environment is recommended before exporting.
        

        \code
        ...
            // --- Main loop
            while (true) {
                if (zed.grab() == SUCCESS) { // Grab an image and computes the tracking
                    Pose cameraPose;
                    zed.getPosition(cameraPose, REFERENCE_FRAME_WORLD);
                }
            }

            // Export the spatial memory for future sessions
            zed.saveCurrentArea("office.area"); // The actual file will be created asynchronously.
            std::cout << zed.getAreaExportState() << std::endl;

            // --- Close the Camera
            zed.close(); // The close method will wait for the end of the file creation using getAreaExportState().
            return 0;
        }
        \endcode
         */
        ERROR_CODE saveCurrentArea(String area_file_path);

        /**
        \brief Returns the state of the spatial memory export process.

        As \ref saveCurrentArea() only starts the exportation, this function allows you to know when the exportation finished or if it failed.

        \return The current \ref AREA_EXPORT_STATE "state" of the spatial memory export process
         */
        AREA_EXPORT_STATE getAreaExportState();

        /**
        \brief Resets the tracking, and re-initializes the position with the given transformation matrix.
        \param path : Position of the camera in the world frame when the function is called. By default, it is set to identity.
        \return \ref ERROR_CODE "ERROR_CODE_FAILURE" if the \ref area_file_path file wasn't found, \ref ERROR_CODE "SUCCESS" otherwise.

        \note Please note that this function will also flush the accumulated or loaded spatial memory.
         */
        ERROR_CODE resetTracking(const Transform &path);

        /**
        \brief Disables the positional tracking.

        The positional tracking is immediately stopped. If a file path is given, \ref saveCurrentArea(area_file_path) will be called asynchronously. See \ref getAreaExportState() to get the exportation state.
        \n If the tracking has been enabled, this function will automatically be called by \ref close() .

        \param area_file_path : if set, saves the spatial memory into an '.area' file. default : (empty)
        \n area_file_path is the name and path of the database, e.g. "path/to/file/myArea1.area".
        \note The '.area' database depends on the depth map SENSING_MODE chosen during the recording. The same mode must be used to reload the database.
         */
        void disableTracking(String area_file_path = "");


        // -----------------------------------------------------------------
        //                        Positional Tracking functions, for ZED-M only (using IMU)
        // -----------------------------------------------------------------

        /**
        \brief Retrieves the IMU Data at a specific time reference

        Calling getIMUData with \ref TIME_REFERENCE "TIME_REFERENCE_CURRENT" gives you the latest IMU data received. Getting all the data requires to call this function at 800Hz in a thread.
        \n Calling getIMUData with \ref TIME_REFERENCE "TIME_REFERENCE_IMAGE" gives you the IMU data at the time of the latest image \ref grab() "grabbed".
        \n 
        \n \ref IMUData object contains 2 category of data:
        \n <b>Time-fused</b> pose estimation that can be accessed using:
        \n   <ul><li>\ref IMUData.getOrientation()</li>
        \n   <li>\ref IMUData.getRotation()</li>
        \n   <li>\ref IMUData.getRotationVector()</li>
        \n   <li>\ref IMUData.getEulerAngles()</li>
        \n   <li>\ref IMUData.pose_data</li>
        \n   <li>\ref IMUData.getRotationMatrix()</li></ul>
        \n <b>Raw values</b> from the IMU sensor:
        \n   <ul><li>\ref IMUData.angular_velocity, corresponding to the gyroscope</li>
        \n   <li>\ref IMUData.linear_acceleration, corresponding to the accelerometer</li></ul>


        \param imu_data : \b [out] the IMUData that inherits from sl::Pose, containing the orientation of the IMU (pose in world reference frame) and other information (timestamp, raw imu data)
        \param reference_time : defines the time reference from when you want the pose to be extracted.
        \return SUCCESS if IMUData has been filled
         
        \n Extract Rotation Matrix : imu_data.getRotation();
        \n Extract Orientation / Quaternion: imu_data.getOrientation();

        \note : Translation is not provided when using the IMU only.
        \note : The quaternion (fused data) is given in the specified COORDINATE_SYSTEM of InitParameters. 
		
		\warning : Until v2.4, the IMU rotation (quaternion, rotation matrix, etc.) was expressed relative to the left camera frame. This means that the camera_imu_transform now given was already applied on the fused quaternion.
		Starting from v2.5, this transformation is given in getCameraInformation().camera_imu_transform and not applied anymore to the fused quaternion, to keep the data in the IMU frame. 
		Therefore, to get the same values calculated in v2.4 with the ZED SDK v2.5 (and up), you will need to apply the transformation as shown in the example below : 

		* \code
		* // Example to get the same quaternion between v2.4 and v2.5
		* /////////////////////////////////////////////////////////
		* // SDK v2.4 : 
		* sl::IMUData imudata;
		* zed.getIMUData(imudata);
		* sl::Orientation quaternionOnImage = imudata.getOrientation(); // quaternion is given in left camera frame
		*
		* // SDK v2.5 and up
		* sl::IMUData imudata;
		* zed.getIMUData(imudata); // quaternion ( imudata.getOrientation() ) is given in IMU frame
		* sl::Transform cIMuMatrix = zed.getCameraInformation().camera_imu_transform;
		* sl::Transform cIMuMatrix_inv = sl::Transform::inverse(cIMuMatrix);
		* sl::Transform dataOnImage = cIMuMatrix * imudata.pose_data * cIMuMatrix_inv;
		* sl::Orientation quaternionOnImage = dataOnImage.getOrientation(); // quaternion is given in left camera frame
		* \endcode
		
        \warning : In SVO reading mode, the TIME_REFERENCE_CURRENT is currently not available (yielding \ref ERROR_CODE "ERROR_CODE_INVALID_FUNCTION_PARAMETERS".
         * Only the quaternion data at TIME_REFERENCE_IMAGE is available. Other values will be set to 0.
         */
        ERROR_CODE getIMUData(IMUData &imu_data, TIME_REFERENCE reference_time);

        /**
        \brief Set an optionnal IMU orientation hint that will be used to assist the tracking during the next \ref grab().

        This function can be used to assist the positional tracking rotation while using a ZED Mini.

        \note This function is only effective if a ZED Mini (ZED-M) is used.
        \n It needs to be called before the \ref grab() function.
        \param sl::Transform to be ingested into IMU fusion. Note that only the rotation is used.
        \return SUCCESS if the transform has been passed, \ref ERROR_CODE "ERROR_CODE_INVALID_FUNCTION_CALL" otherwise (such as when use with the ZED camera due to its lack of an IMU).
         */
        ERROR_CODE setIMUPrior(const sl::Transform& transform);

        //@}

        ///@{
        /// @name Spatial Mapping Functions
        // -----------------------------------------------------------------
        //                         Spatial Mapping functions:
        // -----------------------------------------------------------------

        /**
        \brief Initializes and starts the spatial mapping processes.

        The spatial mapping will create a geometric representation of the scene based on both tracking data and 3D point clouds.
        \n The resulting output is a \ref Mesh and can be obtained by the \ref extractWholeMesh() function or with \ref retrieveMeshAsync() after calling \ref requestMeshAsync().

        \param spatial_mapping_parameters : the structure containing all the specific parameters for the spatial mapping.
        \n Default: a balanced parameter preset between geometric fidelity and output file size. For more information, see the \ref SpatialMappingParameters documentation.
        \return \ref ERROR_CODE "SUCCESS" if everything went fine, \ref ERROR_CODE "ERROR_CODE_FAILURE" otherwise.

        \warning The tracking (\ref enableTracking() ) and the depth (\ref RuntimeParameters.enable_depth ) needs to be enabled to use the spatial mapping.
        \warning The performance greatly depends on the spatial_mapping_parameters.
        \ Lower SpatialMappingParameters.range_meter and SpatialMappingParameters.resolution_meter for higher performance.
        If the mapping framerate is too slow in live mode, consider using an SVO file, or choose a lower mesh resolution.

        \note This features uses host memory (RAM) to store the 3D map. The maximum amount of available memory allowed can be tweaked using the SpatialMappingParameters.
        \n Exeeding the maximum memory allowed immediately stops the mapping.

        \code
        #ifndef NDEBUG
        #error "Spatial mapping requires Release mode, not Debug."
        #endif

        #include <sl/Camera.hpp>

        using namespace sl;

        int main(int argc, char **argv) {

            // Create a ZED camera object
            Camera zed;

            // Set initial parameters
            InitParameters init_params;
            init_params.camera_resolution = RESOLUTION_HD720; // Use HD720 video mode (default fps: 60)
            init_params.coordinate_system = COORDINATE_SYSTEM_RIGHT_HANDED_Y_UP; // Use a right-handed Y-up coordinate system (The OpenGL one)
            init_params.coordinate_units = UNIT_METER; // Set units in meters

            // Open the camera
            ERROR_CODE err = zed.open(init_params);
            if (err != SUCCESS)
                exit(-1);

            // Positional tracking needs to be enabled before using spatial mapping
            sl::TrackingParameters tracking_parameters;
            err = zed.enableTracking(tracking_parameters);
            if (err != SUCCESS)
                exit(-1);

            // Enable spatial mapping
            sl::SpatialMappingParameters mapping_parameters;
            err = zed.enableSpatialMapping(mapping_parameters);
            if (err != SUCCESS)
                exit(-1);

            // Grab data during 500 frames
            int i = 0;
            sl::Mesh mesh; // Create a mesh object
            while (i < 500) {
                // For each new grab, mesh data is updated
                if (zed.grab() == SUCCESS) {
                    // In the background, spatial mapping will use newly retrieved images, depth and pose to update the mesh
                    sl::SPATIAL_MAPPING_STATE mapping_state = zed.getSpatialMappingState();

                    // Print spatial mapping state
                    std::cout << "Images captured: " << i << " / 500  ||  Spatial mapping state: " << spatialMappingState2str(mapping_state) << std::endl;
                    i++;
                }
            }
            std::cout << std::endl;

            // Extract, filter and save the mesh in a obj file
            std::cout << "Extracting Mesh ..." << std::endl;
            zed.extractWholeMesh(mesh); // Extract the whole mesh
            std::cout << "Filtering Mesh ..." << std::endl;
            mesh.filter(sl::MeshFilterParameters::MESH_FILTER_LOW); // Filter the mesh (remove unnecessary vertices and faces)
            std::cout << "Saving Mesh in mesh.obj ..." << std::endl;
            mesh.save("mesh.obj"); // Save the mesh in an obj file


            // Disable tracking and mapping and close the camera
            zed.disableSpatialMapping();
            zed.disableTracking();
            zed.close();
            return 0;
        }
        \endcode
         */
        ERROR_CODE enableSpatialMapping(SpatialMappingParameters spatial_mapping_parameters = SpatialMappingParameters());

        /**
        \brief Returns the current spatial mapping state.

        As the spatial mapping runs asynchronously, this function allows you to get reported errors or status info.

        \return The current state of the spatial mapping process
        \see SPATIAL_MAPPING_STATE
         */
        SPATIAL_MAPPING_STATE getSpatialMappingState();

        // -----------------------------------------------------------------
        // Async functions of mesh generation ( *Async())
        // -----------------------------------------------------------------
        /**
        \brief Starts the mesh generation process in a non blocking thread from the spatial mapping process.

        As Mesh generation can be take a long time depending on the mapping resolution and covered area, this function triggers the generation of a mesh without blocking the program.
        You can get info about the current mesh generation using \ref getMeshRequestStatusAsync(), and retrieve the mesh using \ref retrieveMeshAsync(...) .

        \note Only one mesh generation can be done at a time, consequently while the previous launch is not done every call will be ignored.

        \code
        zed.requestMeshAsync();
        while (zed.getMeshRequestStatusAsync() == ERROR_CODE_FAILURE) {
            //Mesh is still generating
        }

        if (zed.getMeshRequestStatusAsync() == SUCCESS) {
            zed.retrieveMeshAsync(mesh);
            std::cout << "Number of triangles in the mesh: " << mesh.getNumberOfTriangles() << std::endl;
        }
        \endcode
         */
        void requestMeshAsync();

        /**
        \brief Returns the mesh generation status. Useful after calling \ref requestMeshAsync to know if you can call \ref retrieveMeshAsync.
        \return \ref ERROR_CODE "SUCCESS" if the mesh is ready and not yet retrieved, otherwise \ref ERROR_CODE "ERROR_CODE_FAILURE".

        \n See \ref requestMeshAsync() for an example.
         */
        ERROR_CODE getMeshRequestStatusAsync();

        /**
        \brief Retrieves the current generated mesh.

        After calling \ref requestMeshAsync() , this function allows you to retrieve the generated mesh. The mesh will only be available when \ref getMeshRequestStatusAsync() returned \ref ERROR_CODE "SUCCESS"

        \param mesh : \b [out] The mesh to be filled.
        \return \ref ERROR_CODE "SUCCESS" if the mesh is retrieved, otherwise \ref ERROR_CODE "ERROR_CODE_FAILURE".

        \note This function only updates chunks that need to be updated and add the new ones in order to improve update speed.
        \warning You should not modify the mesh between two calls of this function, otherwise it can lead to corrupted mesh.

        \n See \ref requestMeshAsync() for an example.
         */
        ERROR_CODE retrieveMeshAsync(Mesh &mesh);

        // -----------------------------------------------------------------
        // Blocking (synchronous) function of mesh generation
        // -----------------------------------------------------------------
        /**
        \brief Extracts the current mesh from the spatial mapping process.

        If the mesh object to be filled already contains a previous version of the mesh, only changes will be updated, optimizing performance.

        \param mesh : \b [out] The mesh to be filled.
        \return \ref SUCCESS if the mesh is filled and available, otherwise \ref ERROR_CODE_FAILURE.

        \warning This is a blocking function. You should either call it in a thread or at the end of the mapping process.
        \n Is it can be long, calling this function in the grab loop will block the depth and tracking computation and therefore gives bad results.

        \n See \ref enableSpatialMapping() for an example.
         */
        ERROR_CODE extractWholeMesh(Mesh &mesh);

        /**
        \brief Pause or resume the spatial mapping processes.
        
        As spatial mapping runs asynchronously, using this function can pause its computation to free up processing power, and resume it again later.
        \n For example, it can be used to avoid mapping a specifif area or to pause the mapping when the camera is static.
        
        \param status : if true, the integration is paused. If false, the spatial mapping is resumed.
         */
        void pauseSpatialMapping(bool status);

        /**
        \brief Disables the spatial mapping process.

        The spatial mapping is immediately stopped.
        \n If the mapping has been enabled, this function will automatically be called by \ref close().

        \note This function frees the memory allocated for th spatial mapping, consequently, mesh cannot be retrieved after this call.
         */
        void disableSpatialMapping();
        ///@}

        ///@{
        /// @name Plane Detection Functions
        // -----------------------------------------------------------------
        //                         Plane Detection Functions:
        // -----------------------------------------------------------------

        /**
        \brief Checks the plane at the given left image coordinates.
         
         This function gives the 3D plane corresponding to a given pixel in the latest left image \ref grab() "grabbed".
         \n The pixel coordinates are expected to be contained between 0 and \ref getResolution "getResolution().width-1" and \ref getResolution "getResolution().height-1" .

        \param coord : \b [in] The image coordinate. The coordinate must be taken from the full-size image
        \param plane : \b [out] The detected plane if the function succeeded
         \return \ref ERROR_CODE "SUCCESS" if a plane is found otherwise \ref ERROR_CODE "ERROR_CODE_PLANE_NOT_FOUND" .
         \note The reference frame is defined by the \ref RuntimeParameters.measure3D_reference_frame given to the \ref grab() function.
         */
         ERROR_CODE findPlaneAtHit(sl::uint2 coord, sl::Plane &plane);
        
        /**
        \brief Detect the floor plane of the scene
         
         This function analyses the latest image and depth to estimate the floor plane of the scene.
         \n It expects the floor plane to be visible and bigger than other candidate planes, like a table.

        \param floorPlane : \b [out] The detected floor plane if the function succeeded
        \param resetTrackingFloorFrame : \b [out] The transform to align the tracking with the floor plane. The initial position will then 
         * be at ground height, with the axis align with the gravity. The positional tracking needs to be reset/enabled 
         * with this transform as a parameter (TrackingParameters.initial_world_transform)
        \param floor_height_prior : \b [in] Prior set to locate the floor plane depending on the known camera 
         * distance to the ground, expressed in the same unit as the ZED. If the prior is too far from the detected floor plane, 
         * the function will return ERROR_CODE_PLANE_NOT_FOUND
        \param world_orientation_prior : \b [in] Prior set to locate the floor plane depending on the known camera 
         * orientation to the ground. If the prior is too far from the detected floor plane, 
         * the function will return ERROR_CODE_PLANE_NOT_FOUND
        \param floor_height_prior_tolerance : \b [in] Prior height tolerance, absolute value.
        \return \ref ERROR_CODE "SUCCESS" if the floor plane is found and matches the priors (if defined), 
        * otherwise \ref ERROR_CODE "ERROR_CODE_PLANE_NOT_FOUND"
        \note The reference frame is defined by the sl:RuntimeParameters (measure3D_reference_frame)
        * given to the grab() function. The length unit is defined by sl:InitParameters (coordinate_units).
        * With the ZED, the assumption is made that the floor plane is the dominant plane in the scene. The ZED Mini uses the gravity as prior.
        */
        ERROR_CODE findFloorPlane(sl::Plane &floorPlane, sl::Transform &resetTrackingFloorFrame,
                float floor_height_prior = INVALID_VALUE, sl::Rotation world_orientation_prior = sl::Matrix3f::zeros(),
                float floor_height_prior_tolerance = INVALID_VALUE);

        ///@}

        ///@{
        /// @name Recording
        // -----------------------------------------------------------------
        //                 		Recording functions
        // -----------------------------------------------------------------      

        /**
        \brief Creates an SVO file to be filled by \ref record().

        This function can create AVI or SVO files based on the provided filename.
        \n
        \n SVO files are custom video files containing the unrectified images from the camera along with some metadata like timestamps or IMU orientation (if applicable).
        \n They can be used to simulate a live ZED and test a sequence with various SDK parameters.
        \n Depending on the application, various compression modes are available. See \ref SVO_COMPRESSION_MODE.

        \param video_filename : can be a *.svo file or a *.avi file (detected by the suffix name provided).
        \param compression_mode : can be one of the \ref SVO_COMPRESSION_MODE enum. default : SVO_COMPRESSION_MODE_LOSSLESS.
        \return an \ref ERROR_CODE that defines if file was successfully created and can be filled with images.

        \warning This function can be called multiple times during ZED lifetime, but if video_filename is already existing, the file will be erased.
        \note SVO_COMPRESSION_MODE::SVO_COMPRESSION_MODE_RAW is deprecated in recording mode.

        \code
        #include <sl/Camera.hpp>

        using namespace sl;

        int main(int argc, char **argv) {

            // Create a ZED camera object
            Camera zed;

            // Set initial parameters
            InitParameters init_params;
            init_params.camera_resolution = RESOLUTION_HD720; // Use HD720 video mode (default fps: 60)
            init_params.coordinate_units = UNIT_METER; // Set units in meters

            // Open the camera
            ERROR_CODE err = zed.open(init_params);
            if (err != SUCCESS) {
                std::cout << toString(err) << std::endl;
                exit(-1);
            }

            // Enable video recording
            err = zed.enableRecording("myVideoFile.svo", SVO_COMPRESSION_MODE_LOSSLESS);
            if (err != SUCCESS) {
                std::cout << toString(err) << std::endl;
                exit(-1);
            }

            // Grab data during 500 frames
            int i = 0;
            while (i < 500) {
                // Grab a new frame
                if (zed.grab() == SUCCESS) {
                    // Record the grabbed frame in the video file
                    zed.record();
                    i++;
                }
            }

            zed.disableRecording();
            std::cout << "Video has been saved ..." << std::endl;

            zed.close();
            return 0;
        }
        \endcode
         */
        ERROR_CODE enableRecording(String video_filename, SVO_COMPRESSION_MODE compression_mode = SVO_COMPRESSION_MODE_LOSSLESS);

        /**
        \brief Records the current frame provided by \ref grab() into the file.

        Calling this function after a successful \ref grab() call saves the images into the video file opened by \ref enableRecording() .

        \return The recording state structure. For more details, see \ref RecordingState.
        \warning The latest \ref grab() "grabbed" frame will be save, so \ref grab() must be called before.

        \n See \ref enableRecording() for an example.
         */
        RecordingState record();

        /**
        \brief Disables the recording initiated by \ref enableRecording() and closes the generated file.
        
        \note This function will automatically be called by \ref close() if enableRecording() was called.

        See \ref enableRecording() for an example.

         */
        void disableRecording();
        ///@}


        // -----------------------------------------------------------------
        //                         (static)
        // -----------------------------------------------------------------
        /**
        \brief Returns the version of the currently installed ZED SDK.
        \return The ZED SDK version as a string with the following format : MAJOR.MINOR.PATCH

        \code
        std::cout << Camera::getSDKVersion() << std::endl;
        \endcode
         */
        static String getSDKVersion();

        /**
        \brief Returns the number of connected cameras.
        \return The number of connected cameras supported by the SDK. See (\ref MODEL )

        \deprecated See \ref getDeviceList.
        \warning This function has been deprecated in favor of \ref getDeviceList() , which returns more info about the connected devices.
         */
        /*@cond SHOWHIDDEN*/SL_DEPRECATED(/*@endcond*/static int isZEDconnected()/*@cond SHOWHIDDEN*/)/*@endcond*/;

        /**
        \brief List all the connected devices with their associated information.

        This function lists all the cameras available and provides their serial number, models and other information.

        \return The device properties for each connected camera

        \warning As this function returns an std::vector, it is only safe to use in Release mode (not Debug).
        \n This is due to a known compatibility issue between release (the SDK) and debug (your app) implementations of std::vector.

         */
        static std::vector<sl::DeviceProperties> getDeviceList();

        /**
        \brief <b>Only for Nvidia Jetson</b>: Sticks the calling thread to a specific CPU core.
        \param cpuCore : int that defines the core the thread must be run on. Can be between 0 and 3. (cpu0,cpu1,cpu2,cpu3).
        \return \ref SUCCESS if stick is OK, otherwise returns a status error.

        \warning Function only available for Nvidia Jetson. On other platforms, the result will always be 0 and no operations will be performed.
         */
        static ERROR_CODE sticktoCPUCore(int cpu_core);

        /**
        \brief The \ref Camera object cannot be copied. Therfore, its copy constructor is disabled.
        If you need to share a Camera instance across several threads or object, please consider using a pointer.

        \see Camera()
        */
        Camera(const Camera&) = delete;
    private:
        CameraMemberHandler *h = 0;
        bool opened = false;
    };

    /**
    \ingroup Depth_group
    \brief Writes the current depth map into a file.
    \param zed : the current camera object.
    \param format : the depth file format you desired.
    \param name : the name (path) in which the depth will be saved.
    \param factor : only for PNG and PGM, apply a gain to the depth value. default : 1.
    The PNG format only stores integers between 0 and 65536, if you're saving a depth map in meters, values will be rounded to the nearest integer. Set factor to 0.01 to reduce this effect by converting to millimeters.
    Do not forget to scale (by 1./factor) the pixel value at reading to get the real depth.
    The occlusions are represented by 0.
    \return False if something wrong happens, else return true.
     */
    /*@cond SHOWHIDDEN*/SL_SDK_EXPORT/*@endcond*/ bool saveDepthAs(Camera &zed, DEPTH_FORMAT format, String name, float factor = 1.);

    /**
    \ingroup Depth_group
    \brief Writes the current depth map into a file.
    \param depth : the depth map to record (CPU 32F_C1 \ref Mat)
    \param format : the depth file format you desired.
    \param name : the name (path) in which the depth will be saved.
    \param factor : only for PNG and PGM, apply a gain to the depth value. default : 1.
    The PNG format only stores integers between 0 and 65536, if you're saving a depth map in meters, values will be rounded to the nearest integer. Set factor to 0.01 to reduce this effect by converting to millimeters.
    Do not forget to scale (by 1./factor) the pixel value at reading to get the real depth.
    The occlusions are represented by 0.
    \return False if something wrong happens, else return true.
     */
    /*@cond SHOWHIDDEN*/SL_SDK_EXPORT/*@endcond*/ bool saveDepthAs(Mat &depth, DEPTH_FORMAT format, String name, float factor = 1.);

    /**
    \ingroup Depth_group
    \brief Writes the current point cloud into a file
    \param zed : the current camera object.
    \param format : the point cloud file format you desired.
    \param name : the name (path) in which the point cloud will be saved.
    \param with_color : indicates if the color must be saved. default : false.
    \return False if something wrong happens, else return true.

    \note The color is not saved for XYZ and VTK files.
     */
    /*@cond SHOWHIDDEN*/SL_SDK_EXPORT/*@endcond*/ bool savePointCloudAs(Camera &zed, POINT_CLOUD_FORMAT format, String name, bool with_color = false);

    /**
    \ingroup Depth_group
    \brief Writes the current point cloud into a file
    \param cloud : the point cloud to record (CPU 32F_C4 \ref Mat)
    \param format : the point cloud file format you desired.
    \param name : the name (path) in which the point cloud will be saved.
    \param with_color : indicates if the color must be saved. default : false.
    \return False if something wrong happens, else return true.

    \note The color is not saved for XYZ and VTK files.
     */
    /*@cond SHOWHIDDEN*/SL_SDK_EXPORT/*@endcond*/ bool savePointCloudAs(Mat &cloud, POINT_CLOUD_FORMAT format, String name, bool with_color = false);

    /**
    \ingroup SpatialMapping_group
    \brief Converts the given enumerated value into readable text.
    \return The enumerated value as a string.
     */
    String /*@cond SHOWHIDDEN*/SL_SDK_EXPORT/*@endcond*/ toString(const SpatialMappingParameters::MAPPING_RESOLUTION &resolution);
    ///@cond

    inline std::ostream &operator<<(std::ostream &os, const SpatialMappingParameters::MAPPING_RESOLUTION &resolution) {
        return os << toString(resolution);
    }
    ///@endcond

    /**
    \ingroup SpatialMapping_group
    \brief Converts the given enumerated value into readable text.
    \return The enumerated value as a string.
     */
    String /*@cond SHOWHIDDEN*/SL_SDK_EXPORT/*@endcond*/ toString(const SpatialMappingParameters::MAPPING_RANGE &range);
    ///@cond

    inline std::ostream &operator<<(std::ostream &os, const SpatialMappingParameters::MAPPING_RANGE &range) {
        return os << toString(range);
    }
    ///@endcond
}

#endif /* __CAMERA_HPP__ */
