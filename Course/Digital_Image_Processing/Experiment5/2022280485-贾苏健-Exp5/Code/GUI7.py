# TIME : 2024/6/25 19:11

# 在 GUI5 的基础上，新增了滑条，用于调整低通滤波器的强度。
# 新增了保存图像按钮，用于保存处理后的图像。

import sys
import cv2
import random
import numpy as np
from PyQt5.QtCore import Qt, QTimer
from PyQt5.QtGui import QPixmap, QImage
from PIL import Image, ImageOps, ImageFilter, ImageEnhance
from PyQt5.QtWidgets import QApplication, QMainWindow, QLabel, QWidget, QPushButton, QFileDialog, QComboBox, QSlider


class ImageProcessorGUI(QMainWindow):
    def __init__(self):
        super().__init__()

        self.setWindowTitle("贾苏健的 Image Processing Toolbox")
        self.setGeometry(100, 100, 1500, 900)

        self.main_widget = QWidget(self)
        self.setCentralWidget(self.main_widget)

        # 创建模式选择框
        self.mode_selector = QComboBox(self.main_widget)
        self.mode_selector.setGeometry(10, 10, 120, 40)
        self.mode_selector.addItems(["Image Process", "Video Capture"])
        self.mode_selector.currentIndexChanged.connect(self.change_mode)

        # 设置图像显示标签
        self.image_label = QLabel(self.main_widget)
        self.image_label.setGeometry(150, 10, 600, 800)  # x, y, width, height
        self.output_label = QLabel(self.main_widget)
        self.output_label.setGeometry(760, 10, 600, 800)  # x, y, width, height
        self.video_label = QLabel(self.main_widget)
        self.video_label.setGeometry(150, 10, 600, 800)  # x, y, width, height
        self.video_output = QLabel(self.main_widget)
        self.video_output.setGeometry(760, 10, 600, 800)  # x, y, width, height

        self.open_button = self.create_button("Open Image", 10, 60, self.open_image)
        # 创建图像处理模式选择按键
        self.image_selector = QComboBox(self.main_widget)
        self.image_selector.setGeometry(10, 110, 120, 40)
        self.image_selector.addItems(["Color Process", "Filter Process", "Edge Process",  "Enhance Process", "Add noise Process"])
        self.image_selector.currentIndexChanged.connect(self.image_mode_change)
        # 创建颜色处理模式选择按键
        self.color_process_selector = QComboBox(self.main_widget)
        self.color_process_selector.setGeometry(10, 160, 120, 40)
        self.color_process_selector.addItems(["Gray Image", "Binary Image", "Hue Rotation", "Color Inversion"])
        self.color_process_selector.currentIndexChanged.connect(self.image_mode_change)
        # 创建滤波处理模式选择按键
        self.filter_process_selector = QComboBox(self.main_widget)
        self.filter_process_selector.setGeometry(10, 160, 120, 40)
        self.filter_process_selector.addItems(["Low Pass Filter", "High Pass Filter"])
        self.filter_process_selector.currentIndexChanged.connect(self.image_mode_change)
        # 创建边缘处理模式选择按键
        self.edge_process_selector = QComboBox(self.main_widget)
        self.edge_process_selector.setGeometry(10, 160, 120, 40)
        self.edge_process_selector.addItems(["Edge Detection", "Sharpening"])
        self.edge_process_selector.currentIndexChanged.connect(self.image_mode_change)
        # 创建增强处理模式选择按键
        self.enhance_process_selector = QComboBox(self.main_widget)
        self.enhance_process_selector.setGeometry(10, 160, 120, 40)
        self.enhance_process_selector.addItems(["Contrast Enhancement", "Brightness Enhancement", "Saturation Enhancement"])
        self.enhance_process_selector.currentIndexChanged.connect(self.image_mode_change)
        # 创建添加噪声处理模式选择按键
        self.noise_process_selector = QComboBox(self.main_widget)
        self.noise_process_selector.setGeometry(10, 160, 120, 40)
        self.noise_process_selector.addItems(["Salt and pepper noise", "Gaussian noise", "Poisson noise"])
        self.noise_process_selector.currentIndexChanged.connect(self.image_mode_change)

        # self.start_button = self.create_button("Start Camera", 10, 60, self.open_camera)
        # 创建视频处理模式选择按键
        self.video_selector = QComboBox(self.main_widget)
        self.video_selector.setGeometry(10, 60, 120, 40)
        self.video_selector.addItems(["Edge Detection", "Face Detection"])
        self.video_selector.currentIndexChanged.connect(self.video_mode_change)

        # 添加滑条
        self.slider = QSlider(Qt.Horizontal, self.main_widget)
        self.slider.setGeometry(150, 750, 1000, 40)
        self.slider.setRange(1, 100)  # 将范围调整为 1 到 100，增加了滑块的精度
        self.slider.setSingleStep(1)  # 设置每次滑动的步长为 1，使其更连续
        self.slider.valueChanged.connect(self.image_mode_change)
        self.slider.hide()  # 默认隐藏滑条

        # 添加保存图像按钮
        self.save_button = self.create_button("Save Image", 10, 210, self.save_image)

        # 初始化为 Gray Scale 模式
        self.current_image = None
        self.processed_image = None
        self.timer = QTimer()
        self.cap = None
        self.change_mode(0)

    def create_button(self, text, x, y, callback):
        button = QPushButton(text, self.main_widget)
        button.setGeometry(x, y, 120, 40)  # x, y, width, height
        button.clicked.connect(callback)
        return button

    def change_mode(self, index):
        if index == 0:  # Image Process mode
            self.show_image_process_mode()
            self.stop_camera()
        elif index == 1:  # Video Capture mode
            self.show_video_capture_mode()
            self.start_camera()

    def show_image_process_mode(self):
        # Hide video related widgets
        self.video_label.hide()
        self.video_output.hide()
        # self.start_button.hide()
        self.video_selector.hide()

        # Show image processing related widgets
        self.image_label.show()
        self.output_label.show()
        self.open_button.show()
        self.image_selector.show()
        self.save_button.show()
        self.slider.hide()  # Show slider initially to allow selection based on filter type
        self.image_mode_change()  # Apply the initial filter selection to adjust the slider visibility


    def show_video_capture_mode(self):
        # Hide image processing related widgets
        self.image_label.hide()
        self.output_label.hide()
        self.open_button.hide()
        self.image_selector.hide()
        self.color_process_selector.hide()
        self.filter_process_selector.hide()
        self.edge_process_selector.hide()
        self.enhance_process_selector.hide()
        self.noise_process_selector.hide()
        self.slider.hide()
        self.save_button.hide()

        # Show video related widgets
        self.video_label.show()
        self.video_output.show()
        # self.start_button.show()
        self.video_selector.show()

    def open_image(self):
        options = QFileDialog.Options()
        filename, _ = QFileDialog.getOpenFileName(self, "Open Image", "",
                                                  "Image Files (*.png *.jpg *.jpeg *.bmp *.gif)", options=options)
        if filename:
            self.current_image = Image.open(filename)
            self.display_image(self.current_image, self.image_label)
            self.image_mode_change()

    # 用于显示图像
    def display_image(self, image, label):
        if image.mode != "RGB":
            image = image.convert("RGB")
        image = image.convert("RGBA")
        image_data = image.tobytes("raw", "RGBA")
        qimage = QImage(image_data, image.size[0], image.size[1], QImage.Format_RGBA8888)
        pixmap = QPixmap.fromImage(qimage)
        label.setPixmap(pixmap.scaled(label.width(), label.height(), Qt.KeepAspectRatio))

    # 用于更改图像处理模式时调用
    def image_mode_change(self):
        if not self.current_image:
            return

        process_mode = self.image_selector.currentIndex()

        if process_mode == 0:  # Color Process
            self.color_process_selector.show()
            self.filter_process_selector.hide()
            self.edge_process_selector.hide()
            self.enhance_process_selector.hide()
            self.noise_process_selector.hide()
        elif process_mode == 1:  # Filter Process
            self.color_process_selector.hide()
            self.filter_process_selector.show()
            self.edge_process_selector.hide()
            self.enhance_process_selector.hide()
            self.noise_process_selector.hide()
        elif process_mode == 2:  # Edge Process
            self.color_process_selector.hide()
            self.filter_process_selector.hide()
            self.edge_process_selector.show()
            self.enhance_process_selector.hide()
            self.noise_process_selector.hide()
        elif process_mode == 3:  # Enhance Process
            self.color_process_selector.hide()
            self.filter_process_selector.hide()
            self.edge_process_selector.hide()
            self.enhance_process_selector.show()
            self.noise_process_selector.hide()
        elif process_mode == 4:  # Add noise Process
            self.color_process_selector.hide()
            self.filter_process_selector.hide()
            self.edge_process_selector.hide()
            self.enhance_process_selector.hide()
            self.noise_process_selector.show()

        if process_mode == 0:  # Color Process
            color_process_mode = self.color_process_selector.currentIndex()
            if color_process_mode == 0:
                self.slider.hide()
                self.gray_scale()
            elif color_process_mode == 1:
                self.slider.show()
                self.binary_image()
            elif color_process_mode == 2:
                self.slider.show()
                self.hue_rotation()
            elif color_process_mode == 3:
                self.slider.hide()
                self.color_inversion()
        elif process_mode == 1:  # Filter Process
            self.slider.show()
            filter_process_mode = self.filter_process_selector.currentIndex()
            if filter_process_mode == 0:
                self.low_pass_filter()
            elif filter_process_mode == 1:
                self.high_pass_filter()
        elif process_mode == 2:  # Edge Process
            edge_process_mode = self.edge_process_selector.currentIndex()
            if edge_process_mode == 0:
                self.slider.hide()
                self.edge_detection()
            elif edge_process_mode == 1:
                self.slider.show()
                self.sharpening()
            # elif edge_process_mode == 2:
            #     self.face_detection()
        elif process_mode == 3:  # Enhance Process
            self.slider.show()
            enhance_process_mode = self.enhance_process_selector.currentIndex()
            if enhance_process_mode == 0:
                self.contrast_enhancement()
            elif enhance_process_mode == 1:
                self.brightness_enhancement()
            elif enhance_process_mode == 2:
                self.saturation_enhancement()
        elif process_mode == 4:  # Add noise Process
            self.slider.show()
            add_noise_process_mode = self.noise_process_selector.currentIndex()
            if add_noise_process_mode == 0:
                self.add_salt_and_pepper_noise()
            elif add_noise_process_mode == 1:
                self.add_gaussian_noise()
            elif add_noise_process_mode == 2:
                self.add_poisson_noise()


    # 用于更改视频处理模式时调用
    def video_mode_change(self):
        if not self.cap or not self.cap.isOpened():
            return

        # 不要在这里更新摄像头帧，而是确保定时器连接到update_camera_frame
        self.update_camera_frame()

    def gray_scale(self):
        if self.current_image:
            gray_image = ImageOps.grayscale(self.current_image)
            self.display_image(gray_image, self.output_label)
            self.processed_image = gray_image

    def binary_image(self):
        if self.current_image:
            threshold = self.slider.value() * 255 / 100  # Retrieve slider value and scale to [0, 255]
            grayscale_image = self.current_image.convert("L")  # 转换为灰度图像
            binary_image = grayscale_image.point(lambda p: p > threshold and 255)  # 应用阈值处理

            self.display_image(binary_image, self.output_label)
            self.processed_image = binary_image

    def hue_rotation(self):
        if self.current_image:
            hue_factor = self.slider.value() / 20.0  # Retrieve slider value
            enhanced_image = ImageEnhance.Color(self.current_image).enhance(hue_factor)

            self.display_image(enhanced_image, self.output_label)
            self.processed_image = enhanced_image

    def color_inversion(self):
        if self.current_image:
            inverted_image = ImageOps.invert(self.current_image)
            self.display_image(inverted_image, self.output_label)
            self.processed_image = inverted_image

    def low_pass_filter(self):
        if self.current_image:
            # 根据滑条的值设置滤波器强度
            strength = self.slider.value() / 10.0
            filtered_image = self.current_image.filter(ImageFilter.GaussianBlur(radius=strength))
            self.display_image(filtered_image, self.output_label)
            self.processed_image = filtered_image

    def high_pass_filter(self):
        if self.current_image:
            strength = self.slider.value()  # 获取滑条当前值作为滤波器强度
            filtered_image = self.current_image.filter(ImageFilter.FIND_EDGES)
            filtered_image = filtered_image.filter(ImageFilter.UnsharpMask(radius=strength * 10.0, percent=150, threshold=3))
            self.display_image(filtered_image, self.output_label)
            self.processed_image = filtered_image

    def edge_detection(self):
        if self.current_image:
            filtered_image = self.current_image.filter(ImageFilter.FIND_EDGES)
            self.display_image(filtered_image, self.output_label)
            self.processed_image = filtered_image

    # def face_detection(self):
    #     if self.current_image:
    #         face_cascade = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_frontalface_default.xml')
    #         frame = cv2.cvtColor(cv2.imread("tmp.jpg"), cv2.COLOR_BGR2GRAY)
    #         faces = face_cascade.detectMultiScale(frame, 1.3, 5)
    #         for (x, y, w, h) in faces:
    #             cv2.rectangle(frame, (x, y), (x + w, y + h), (255, 0, 0), 2)
    #         self.display_image(frame, self.output_label)
    #         self.processed_image = frame

    def sharpening(self):
        if self.current_image:
            strength = self.slider.value() / 10.0  # 获取滑条当前值并缩放到合适范围
            sharpened_image = self.current_image.filter(ImageFilter.SHARPEN)
            sharpened_image = ImageEnhance.Sharpness(sharpened_image).enhance(strength)
            self.display_image(sharpened_image, self.output_label)
            self.processed_image = sharpened_image

    def contrast_enhancement(self):
        if self.current_image:
            strength = self.slider.value() / 50.0 + 1.0  # 获取滑条当前值并缩放到合适范围
            enhanced_image = ImageEnhance.Contrast(self.current_image).enhance(strength)
            self.display_image(enhanced_image, self.output_label)
            self.processed_image = enhanced_image

    def brightness_enhancement(self):
        if self.current_image:
            strength = self.slider.value() / 50.0 + 1.0  # 获取滑条当前值并缩放到合适范围
            enhanced_image = ImageEnhance.Brightness(self.current_image).enhance(strength)
            self.display_image(enhanced_image, self.output_label)
            self.processed_image = enhanced_image

    def saturation_enhancement(self):
        if self.current_image:
            strength = self.slider.value() / 50.0 + 1.0  # 获取滑条当前值并缩放到合适范围
            enhanced_image = ImageEnhance.Color(self.current_image).enhance(strength)
            self.display_image(enhanced_image, self.output_label)
            self.processed_image = enhanced_image

    def add_salt_and_pepper_noise(self):
        if self.current_image:
            strength = self.slider.value() / 10.0  # Retrieve slider value
            if self.current_image.mode != "RGB":
                image = self.current_image.convert("RGB")
            else:
                image = self.current_image.copy()

            width, height = image.size
            pixels = image.load()
            num_pixels = int(width * height * (strength / 100.0))  # Adjust noise intensity based on slider (range 1-10)

            for _ in range(num_pixels):
                x = random.randint(0, width - 1)
                y = random.randint(0, height - 1)
                if random.random() < 0.5:
                    pixels[x, y] = (0, 0, 0)  # Pepper noise, black
                else:
                    pixels[x, y] = (255, 255, 255)  # Salt noise, white

            self.display_image(image, self.output_label)
            self.processed_image = image

    def add_gaussian_noise(self):
        if self.current_image:
            strength = self.slider.value() / 10.0 * 25  # Retrieve slider value and scale to [0, 250]
            noisy_image = self.current_image.copy()
            width, height = noisy_image.size
            noise = np.random.normal(0, strength, (height, width, 3))
            noisy_image = np.array(noisy_image) + noise
            noisy_image = np.clip(noisy_image, 0, 255)
            noisy_image = Image.fromarray(noisy_image.astype('uint8'), 'RGB')

            self.display_image(noisy_image, self.output_label)
            self.processed_image = noisy_image

    def add_poisson_noise(self):
        if self.current_image:
            strength = 10.1 - self.slider.value() / 10.0  # Retrieve slider value
            noisy_image = self.current_image.copy()

            # Convert to numpy array and apply Poisson noise to each channel
            noisy_array = np.array(noisy_image)
            for channel in range(noisy_array.shape[2]):
                noisy_array[:, :, channel] = np.random.poisson(noisy_array[:, :, channel] * strength) / strength

            # Clip values to [0, 255] and convert back to uint8
            noisy_array = np.clip(noisy_array, 0, 255).astype(np.uint8)

            # Convert NumPy array back to PIL Image
            noisy_image = Image.fromarray(noisy_array)

            # Display the noisy image
            self.display_image(noisy_image, self.output_label)
            self.processed_image = noisy_image

    def save_image(self):
        if self.processed_image:
            options = QFileDialog.Options()
            save_filename, _ = QFileDialog.getSaveFileName(self, "Save Image", "",
                                                           "Image Files (*.png *.jpg *.jpeg *.bmp *.gif)",
                                                           options=options)
            if save_filename:
                self.processed_image.save(save_filename)

    # def open_camera(self):
    #     if not self.cap:
    #         self.cap = cv2.VideoCapture(0)
    #     if not self.timer.isActive():
    #         self.timer.timeout.connect(self.update_camera_frame)

    def start_camera(self):
        if not self.cap:
            self.cap = cv2.VideoCapture(0)
        if not self.timer.isActive():
            self.timer.timeout.connect(self.update_camera_frame)  # 连接定时器的槽函数
            self.timer.start(30)  # 30ms

    def stop_camera(self):
        if self.timer.isActive():
            self.timer.stop()
        if self.cap:
            self.cap.release()
            self.cap = None
        self.video_label.clear()
        self.video_output.clear()

    def update_camera_frame(self):
        if self.cap:
            ret, frame = self.cap.read()
            if ret:
                self.display_video_frame(frame, self.video_label)
                # 根据选择的视频处理模式进行处理
                video_process = self.video_selector.currentIndex()
                if video_process == 0:
                    self.video_edge_detection(frame)
                elif video_process == 1:
                    self.video_face_detection(frame)

    def video_edge_detection(self, frame):
        edges = cv2.Canny(frame, 100, 200)
        self.display_video_frame(edges, self.video_output, is_gray=True)

    def video_face_detection(self, frame):
        face_cascade = cv2.CascadeClassifier(cv2.data.haarcascades + 'haarcascade_frontalface_default.xml')
        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        faces = face_cascade.detectMultiScale(gray, 1.3, 5)
        for (x, y, w, h) in faces:
            cv2.rectangle(frame, (x, y), (x + w, y + h), (255, 0, 0), 2)
        self.display_video_frame(frame, self.video_output)

    # 此函数只用于展示视频
    def display_video_frame(self, frame, label, is_gray=False):
        if is_gray:
            qimage = QImage(frame.data, frame.shape[1], frame.shape[0], QImage.Format_Grayscale8)
        else:
            rgb_image = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
            qimage = QImage(rgb_image.data, rgb_image.shape[1], rgb_image.shape[0], QImage.Format_RGB888)
        pixmap = QPixmap.fromImage(qimage)
        label.setPixmap(pixmap.scaled(label.width(), label.height(), Qt.KeepAspectRatio))


if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = ImageProcessorGUI()
    window.show()
    sys.exit(app.exec_())
