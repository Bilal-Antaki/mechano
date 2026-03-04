import rclpy
from rclpy.node import Node
from sensor_msgs.msg import Image
from cv_bridge import CvBridge
import cv2


class CameraProcessor(Node):
    def __init__(self):
        super().__init__('camera_processor')

        self.bridge = CvBridge()

        self.sub = self.create_subscription(
            Image,
            '/camera/image_raw',
            self.image_callback,
            10
        )

        self.pub = self.create_publisher(Image, '/mechano_vision/processed_image', 10)

        self.get_logger().info('Camera processor node started.')

    def image_callback(self, msg: Image):
        # Convert ROS Image to OpenCV BGR frame
        frame = self.bridge.imgmsg_to_cv2(msg, desired_encoding='bgr8')

        # --- Your CV processing goes here ---
        processed = self.process(frame)
        # ------------------------------------

        # Publish processed image back as ROS Image
        out_msg = self.bridge.cv2_to_imgmsg(processed, encoding='bgr8')
        out_msg.header = msg.header
        self.pub.publish(out_msg)

    def process(self, frame):
        """Override or extend this method with your CV logic."""
        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        # Convert back to BGR so the output topic is consistent
        return cv2.cvtColor(gray, cv2.COLOR_GRAY2BGR)


def main(args=None):
    rclpy.init(args=args)
    node = CameraProcessor()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()


if __name__ == '__main__':
    main()
