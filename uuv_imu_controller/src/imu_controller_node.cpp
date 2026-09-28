// IMU -> velocity command node.
//
// Subscribes : /imu/data         (sensor_msgs/msg/Imu)
// Publishes  : /input/command    (geometry_msgs/msg/Twist)
//
// Search for "TODO" to find the parts you need to write.

#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/imu.hpp"
#include "geometry_msgs/msg/twist.hpp"

namespace
{
constexpr char IMU_TOPIC[] = "/imu/data";
constexpr char CMD_TOPIC[] = "/input/command";
constexpr size_t QUEUE_SIZE = 10;
}  // namespace

// Plain container for the raw IMU values we care about.
struct ImuData
{
  // Linear acceleration [m/s^2]
  double accel_x = 0.0;
  double accel_y = 0.0;
  double accel_z = 0.0;
  // Angular velocity [rad/s]
  double gyro_x = 0.0;
  double gyro_y = 0.0;
  double gyro_z = 0.0;
};

class ImuControllerNode : public rclcpp::Node
{
public:
  ImuControllerNode()
  : Node("imu_controller")
  {
    // TODO 1: Create a subscription to IMU_TOPIC and store it in imu_sub_.
    //   - message type: sensor_msgs::msg::Imu
    //   - queue size:   QUEUE_SIZE
    //   - callback:     imu_callback
    //   Hint: this->create_subscription<...>(topic, qos, callback)
    //         For the callback, look up std::bind(..., this, std::placeholders::_1)
    //         or use a lambda that captures `this`.

    // TODO 2: Create a publisher on CMD_TOPIC and store it in cmd_pub_.
    //   - message type: geometry_msgs::msg::Twist
    //   - queue size:   QUEUE_SIZE
    //   Hint: this->create_publisher<...>(topic, qos)

    RCLCPP_INFO(get_logger(), "imu_controller started: %s -> %s", IMU_TOPIC, CMD_TOPIC);
  }

private:
  // Called every time a new IMU message arrives.
  void imu_callback(const sensor_msgs::msg::Imu::SharedPtr msg)
  {
    // TODO 3: Copy the raw readings from `msg` into imu_data_.
    //   Hint: look at the Imu message definition:
    //     ros2 interface show sensor_msgs/msg/Imu
    //   You want msg->linear_acceleration.{x,y,z} and
    //   msg->angular_velocity.{x,y,z}.

    geometry_msgs::msg::Twist cmd = compute_command(imu_data_);

    // TODO 5: Publish `cmd` using the publisher you created in TODO 2.
  }

  // The fun part: decide how the vehicle should move given the IMU reading.
  //
  // Twist fields:
  //   cmd.linear.x   surge  (forward / backward)
  //   cmd.linear.y   sway   (left / right)
  //   cmd.linear.z   heave  (up / down)
  //   cmd.angular.x  roll
  //   cmd.angular.y  pitch
  //   cmd.angular.z  yaw
  //
  // Anything you leave untouched stays 0.0.
  geometry_msgs::msg::Twist compute_command(const ImuData & imu)
  {
    geometry_msgs::msg::Twist cmd;

    // TODO 4: Design your own mapping from `imu` to `cmd`.
    //   See the README for ideas. Keep the output bounded and
    //   think about what should happen when the IMU is sitting still.

    return cmd;
  }

  ImuData imu_data_;
  rclcpp::Subscription<sensor_msgs::msg::Imu>::SharedPtr imu_sub_;
  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr cmd_pub_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<ImuControllerNode>());
  rclcpp::shutdown();
  return 0;
}
