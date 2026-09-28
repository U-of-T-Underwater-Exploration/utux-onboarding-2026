# uuv_imu_controller

An **onboarding project** for practising basic ROS 2 subscribing and publishing.

You'll finish a single node that:

1. **subscribes** to raw IMU readings on `/imu/data` (linear acceleration and angular velocity in x, y and z)
2. turns those readings into a movement decision
3. **publishes** a velocity command (`geometry_msgs/msg/Twist`) on `/input/command`

The ROS plumbing is small on purpose. The creative part is **designing a fun interface that translates an IMU reading into how the vehicle should move**. For example, tilting the IMU could steer the vehicle, or shaking it could make it dive. You decide.

## What you'll learn

- How a ROS 2 C++ package is laid out (`package.xml`, `CMakeLists.txt`)
- Creating a subscriber and a publisher with `rclcpp`
- Reading fields out of a standard message (`sensor_msgs/Imu`) and filling one in (`geometry_msgs/Twist`)
- Building with `colcon`, then running and debugging with the `ros2` CLI

## Package layout

```
uuv_imu_controller/
├── package.xml                     # package metadata and dependencies
├── CMakeLists.txt                  # build config; builds the `imu_controller` executable
├── launch/imu_controller.launch.py # launches the node
└── src/
    └── imu_controller_node.cpp     # <-- your work goes here
```

## Your tasks

Open `uuv_imu_controller/src/imu_controller_node.cpp` and complete the `TODO`s:

| TODO | What to do |
|------|------------|
| 1 | Create the subscription to `/imu/data` (`sensor_msgs/msg/Imu`) |
| 2 | Create the publisher on `/input/command` (`geometry_msgs/msg/Twist`) |
| 3 | In `imu_callback`, copy the incoming message into the `ImuData` object |
| 4 | In `compute_command`, design your IMU-to-motion mapping **(the fun part)** |
| 5 | Publish the resulting command |

The node already builds and runs as it is. Until you finish the TODOs it just won't do anything useful, and the compiler will warn about unused variables. Those warnings go away as you fill things in.

### Data you're working with

The IMU values are stored in this helper struct:

```cpp
struct ImuData {
  double accel_x, accel_y, accel_z;  // linear acceleration [m/s^2]
  double gyro_x,  gyro_y,  gyro_z;   // angular velocity    [rad/s]
};
```

The command you publish is a `Twist`:

| Field | Meaning |
|-------|---------|
| `linear.x`  | surge (forward/back) |
| `linear.y`  | sway (left/right) |
| `linear.z`  | heave (up/down) |
| `angular.x` | roll rate |
| `angular.y` | pitch rate |
| `angular.z` | yaw rate |

## Interface ideas

Pick one, mix several, or come up with your own:

- **Tilt-to-drive:** tilt forward/back to surge, tilt left/right to sway. When the IMU is flat and still, gravity shows up almost entirely on `accel_z` (about 9.81).
- **Twist-to-turn:** spin the IMU about its vertical axis (`gyro_z`) to yaw the vehicle.
- **Shake-to-dive:** a sharp spike in acceleration toggles diving or surfacing.
- **Gesture modes:** a quick flick switches between "drive" and "hover" modes.
- **Mirror mode:** copy the IMU's angular velocity straight to the vehicle's angular velocity.

Things worth thinking about:

- **Deadband:** ignore tiny readings so the vehicle doesn't drift from sensor noise.
- **Scaling and clamping:** keep commands in a sensible range, e.g. `[-1.0, 1.0]`.
- **Gravity:** it's always in the accelerometer reading. How will you handle that?
- **Smoothing:** raw IMU data is noisy. A simple moving average or low-pass filter helps.

## Build

From the root of your ROS 2 workspace (the folder that contains `src/`):

```bash
colcon build --packages-select uuv_imu_controller
source install/setup.bash
```

This is C++, so you need to re-run `colcon build` (and re-source) after every code change.

## Run

```bash
ros2 run uuv_imu_controller imu_controller
# or
ros2 launch uuv_imu_controller imu_controller.launch.py
```

## Test it without real hardware

In a second terminal, publish a fake IMU reading. This one is "tilted forward and slowly yawing":

```bash
ros2 topic pub -r 10 /imu/data sensor_msgs/msg/Imu \
  "{linear_acceleration: {x: 3.0, y: 0.0, z: 9.3}, angular_velocity: {x: 0.0, y: 0.0, z: 0.5}}"
```

In a third terminal, watch what your node sends out:

```bash
ros2 topic echo /input/command
```

Other useful commands:

```bash
ros2 node list
ros2 topic list
ros2 topic info /input/command      # should show 1 publisher once TODO 2 is done
ros2 topic hz /input/command
ros2 interface show sensor_msgs/msg/Imu
ros2 interface show geometry_msgs/msg/Twist
```

## Stretch goals

- Expose your gains and deadband as **ROS parameters** (`declare_parameter(...)`) so they can be tuned without editing code.
- Publish at a fixed rate with a **timer** instead of on every IMU message.
- Add a **safety timeout**: publish a zero command if no IMU data has arrived for 0.5 s.
- Write a small **unit test** for `compute_command` with `ament_cmake_gtest`.

## Resources

- [ROS 2 tutorial: Writing a simple publisher and subscriber (C++)](https://docs.ros.org/en/humble/Tutorials/Beginner-Client-Libraries/Writing-A-Simple-Cpp-Publisher-And-Subscriber.html)
- [sensor_msgs/Imu](https://docs.ros2.org/latest/api/sensor_msgs/msg/Imu.html)
- [geometry_msgs/Twist](https://docs.ros2.org/latest/api/geometry_msgs/msg/Twist.html)
