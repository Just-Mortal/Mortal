# TIME : 2023/11/21 0:56

import cv2
import numpy as np
import math

min_tre = 50000

"""读取图像"""
image = cv2.imread('Imagine_data/image.png', cv2.IMREAD_COLOR)
copy_image = cv2.imread('Imagine_data/image.png', cv2.IMREAD_COLOR)
thresholded_image = cv2.imread('Result_image/thresholded_image.png')


"""两点之间距离"""
def get_distance(x1, y1, x2, y2):
    return np.sqrt(np.power(x1 - x2, 2) + np.power(y1 - y2, 2))


"""点到线距离"""
def get_distance_point2line(point, line_ab):
    k_line, b = line_ab
    distance = abs(k_line * point[0] - point[1] + b) / math.sqrt(k_line ** 2 + 1)
    return distance


def draw_rec(center, wu, wd, hu, hd, angel):
    x = center[0]
    y = center[1]
    angelPi = (angel / 180) * math.pi + math.pi

    x1 = x + wu * math.cos(angelPi) - hu * math.sin(angelPi)
    y1 = y + wu * math.sin(angelPi) + hu * math.cos(angelPi)

    x2 = x + wu * math.cos(angelPi) + hd * math.sin(angelPi)
    y2 = y + wu * math.sin(angelPi) - hd * math.cos(angelPi)

    x3 = x - wd * math.cos(angelPi) + hu * math.sin(angelPi)
    y3 = y - wd * math.sin(angelPi) - hu * math.cos(angelPi)

    x4 = x - wd * math.cos(angelPi) - hd * math.sin(angelPi)
    y4 = y - wd * math.sin(angelPi) + hd * math.cos(angelPi)

    cv2.line(image, (int(x1), int(y1)), (int(x2), int(y2)), (144, 144, 0), thickness=2)
    cv2.line(image, (int(x2), int(y2)), (int(x3), int(y3)), (144, 144, 0), thickness=2)
    cv2.line(image, (int(x3), int(y3)), (int(x4), int(y4)), (144, 144, 0), thickness=2)
    cv2.line(image, (int(x4), int(y4)), (int(x1), int(y1)), (144, 144, 0), thickness=2)


class Point(object):
    def __init__(self, x, y):
        self.x = x
        self.y = y

    def getX(self):
        return self.x

    def getY(self):
        return self.y


"""获取图像大小信息"""
image_size = np.shape(image)[0:2]

"""转化为灰度图"""
chanel_B = image[:, :, 0]
chanel_G = image[:, :, 1]
chanel_R = image[:, :, 2]
gray_image = 0.299 * chanel_R + 0.587 * chanel_G + 0.114 * chanel_B

"""计算灰度均值并获取二值图像"""
gray_pixel_total = 0
for row in gray_image:
    for pixel in row:
        gray_pixel_total += pixel

gray_pixel_mean = gray_pixel_total / (gray_image.shape[0] * gray_image.shape[1])  # 计算图像的平均灰度值作为阈值
print("灰度图像像素均值 =", gray_pixel_mean)

thresholded_image = np.zeros_like(gray_image)
for i in range(gray_image.shape[0]):
    for j in range(gray_image.shape[1]):
        if gray_image[i, j] > gray_pixel_mean:
            thresholded_image[i, j] = 255
        else:
            thresholded_image[i, j] = 0

"""联通组标记 begin"""
white_pixel_val = 0
black_pixel_val = 255
tem_image = np.zeros(image_size)
label = 1
flag = 0

object_num = 0
object_lab = 0
same_lab = 0

for x in range(1, image_size[0] - 1):
    for y in range(1, image_size[1] - 1):
        print("标记进度：", (x * (image_size[1] - 2) + y) * 100 / ((image_size[0] - 2) * (image_size[1] - 2)), "%")

        # case 1
        if thresholded_image[x, y] == black_pixel_val:
            tem_image[x, y] = black_pixel_val
            if flag == 1:
                label += 1
                flag = 0
                if label == 255:
                    label += 1
        else:
            # case 2
            if thresholded_image[x - 1, y] == black_pixel_val \
                    and thresholded_image[x, y - 1] == black_pixel_val \
                    and thresholded_image[x - 1, y - 1] == black_pixel_val:

                tem_image[x, y] = label
                flag = 1
                same_lab = 0

            # case3
            elif thresholded_image[x - 1, y - 1] != black_pixel_val:

                tem_image[x, y] = tem_image[x - 1, y - 1]
                flag = 1

            # case4
            elif thresholded_image[x - 1, y] == black_pixel_val \
                    and thresholded_image[x - 1, y - 1] == black_pixel_val \
                    and thresholded_image[x, y - 1] != black_pixel_val:

                tem_image[x, y] = tem_image[x, y - 1]
                flag = 1

            # case5
            elif thresholded_image[x, y - 1] == black_pixel_val \
                    and thresholded_image[x - 1, y - 1] == black_pixel_val \
                    and thresholded_image[x - 1, y] != black_pixel_val:

                tem_image[x, y] = tem_image[x - 1, y]
                flag = 1

            # case6
            elif thresholded_image[x - 1, y] != black_pixel_val \
                    and thresholded_image[x - 1, y - 1] == black_pixel_val \
                    and thresholded_image[x, y - 1] != black_pixel_val:

                tem_image[x, y] = tem_image[x, y - 1]
                flag = 1

for x in range(0, image_size[0]):
    for y in range(0, image_size[1]):
        if tem_image[x, y] == 0:
            tem_image[x, y] = 255

connects = [Point(-1, -1), Point(0, -1), Point(1, -1), Point(1, 0),
            Point(1, 1), Point(0, 1), Point(-1, 1), Point(-1, 0)]
img_mark = np.zeros(image_size)
seed_list = []
lab_list = []

for x in range(1, image_size[0] - 1):
    for y in range(1, image_size[1] - 1):
        print("统一联通体：", (x * (image_size[1] - 2) + y) * 100 / ((image_size[0] - 2) * (image_size[1] - 2)), "%")
        if tem_image[x, y] != 255 and img_mark[x, y] == 0:
            seed_list.append(Point(x, y))
            while len(seed_list) > 0:
                seed_tmp = seed_list[0]
                seed_list.pop(0)
                for i in range(8):
                    tmpX = seed_tmp.x + connects[i].x
                    tmpY = seed_tmp.y + connects[i].y
                    tmpLabel = int(tem_image[x, y])
                    if img_mark[tmpX, tmpY] != 1 \
                            and tem_image[tmpX, tmpY] != black_pixel_val \
                            and tem_image[tmpX, tmpY] != white_pixel_val:

                        tem_image[tmpX, tmpY] = tmpLabel
                        img_mark[tmpX, tmpY] = 1
                        seed_list.append(Point(tmpX, tmpY))

                        if tmpLabel not in lab_list:
                            lab_list.append(tmpLabel)
"""联通组标记 end"""

B = 0
G = 155
R = 255

length = len(lab_list)
avg_x = np.zeros(length, int)
avg_y = np.zeros(length, int)
avg_x_cnt = np.zeros(length, int)
avg_y_cnt = np.zeros(length, int)
a_pre = 0
b_pre = 0
c_pre = 0

print("寻找几何中心中... ...")

for x in range(0, image_size[0]):
    for y in range(0, image_size[1]):

        print("几何中心寻找进度：", (x * (image_size[1] - 2) + y) * 100 / ((image_size[0] - 2) * (image_size[1] - 2)), "%")

        if tem_image[x, y] in lab_list:
            avg_x[lab_list.index(tem_image[x, y])] += x
            avg_x_cnt[lab_list.index(tem_image[x, y])] += 1

            avg_y[lab_list.index(tem_image[x, y])] += y
            avg_y_cnt[lab_list.index(tem_image[x, y])] += 1

            a_pre += x ** 2

for i in range(length):
    if avg_x[i] < min_tre or avg_y[i] < min_tre:
        avg_x[i] = -5
        avg_y[i] = -5

avg_x = avg_x / avg_x_cnt
avg_y = avg_y / avg_y_cnt

avg_x = avg_x.tolist()
avg_y = avg_y.tolist()

print("标明几何中心中... ...")

for i in range(length):
    tmpX = int(avg_x[i])
    tmpY = int(avg_y[i])
    image[tmpX, tmpY, 0] = 0
    image[tmpX, tmpY, 1] = 0
    image[tmpX, tmpY, 2] = 255
    for j in range(8):
        for k in range(8):
            tmpX = int(avg_x[i]) + connects[j].x + connects[k].x
            tmpY = int(avg_y[i]) + connects[j].y + connects[k].y
            if tmpX < 0 or tmpY < 0 or tmpX >= image_size[0] or tmpY >= image_size[1]:
                continue
            image[tmpX, tmpY, 0] = 0
            image[tmpX, tmpY, 1] = 0
            image[tmpX, tmpY, 2] = 255

new_list = np.zeros(length, int)
new_list = new_list.tolist()
tem_cnt = 0

for i in range(length):
    if avg_x[i] > 0 and avg_y[i] > 0:
        new_list[tem_cnt] = lab_list[i]
        tem_cnt += 1

print("绘制边界中... ...")

for x in range(0, image_size[0]):
    for y in range(0, image_size[1]):

        if tem_image[x, y] != 255 and tem_image[x, y] in new_list:
            if tem_image[x - 1, y] != tem_image[x, y] \
                    or tem_image[x + 1, y] != tem_image[x, y] \
                    or tem_image[x, y - 1] != tem_image[x, y] \
                    or tem_image[x, y + 1] != tem_image[x, y]:
                
                image[x, y, 0] = B
                image[x, y, 1] = G
                image[x, y, 2] = R

                image[x + 1, y, 0] = B
                image[x + 1, y, 1] = G
                image[x + 1, y, 2] = R

                image[x - 1, y, 0] = B
                image[x - 1, y, 1] = G
                image[x - 1, y, 2] = R

                image[x, y + 1, 0] = B
                image[x, y + 1, 1] = G
                image[x, y + 1, 2] = R

output = np.hstack((copy_image, image))

x_min_value = np.zeros(length, int)
x_max_value = np.zeros(length, int)
y_min_value = np.zeros(length, int)
y_max_value = np.zeros(length, int)

for i in range(length):

    print("边界方向寻找进度：", i/length*100, "%")

    x_min_value[i] = 9999
    y_min_value[i] = 9999
    x_max_value[i] = 0
    y_max_value[i] = 0

tem_cnt = [image_size[0], 0, image_size[1], 0]  # x_min x_max y_min y_max

img_tem_rota = copy_image
area = np.zeros(length, float)
area_min_angle = np.zeros(length, float)
min_rect_long_up = np.zeros(length, float)
min_rect_width_up = np.zeros(length, float)
min_rect_long_down = np.zeros(length, float)
min_rect_width_down = np.zeros(length, float)
tem_area = 0
cnt = 0

for i in range(length):
    print("边界框方向确定进度：", i / length * 100, "%")
    if avg_x[i] < 0 or avg_y[i] < 0:
        continue

    area[i] = image_size[0] * image_size[1]
    center = (image_size[0], image_size[1])

    # 平移 先将目标平移到屏幕中心，再开始旋转
    ty = (-1) * (avg_x[i] - image_size[1])
    tx = (-1) * (avg_y[i] - image_size[0])

    translation_matrix = np.array([
        [1, 0, tx],
        [0, 1, ty]
    ], dtype=np.float32)
    translation_image = cv2.warpAffine(src=tem_image, M=translation_matrix, dsize=(image_size[0] * 2, image_size[1] * 2))

    # 遍历每一个角度
    for rotated_angle in range(0, 90, 10):
        print("寻找合适的角度", rotated_angle, end=";")
        # 旋转
        rotate_matrix = cv2.getRotationMatrix2D(center=center, angle=rotated_angle, scale=1)
        rotated_image = cv2.warpAffine(src=translation_image, M=rotate_matrix, dsize=(image_size[0] * 2, image_size[1] * 2))
        tem_image_size = np.shape(rotated_image)[0:2]
        # 每个角度对应一幅图也要清零
        x_min_value[i] = 9999
        y_min_value[i] = 9999
        x_max_value[i] = 0
        y_max_value[i] = 0

        for x in range(100, tem_image_size[0] - 100):
            for y in range(100, tem_image_size[1] - 100):
                if lab_list[i] == rotated_image[x, y]:
                    if x < 0 or y < 0:
                        continue
                    if lab_list[i] != rotated_image[x - 1, y]:
                        if x < x_min_value[i]:
                            x_min_value[i] = x
                    elif lab_list[i] != rotated_image[x + 1, y]:
                        if x > x_max_value[i]:
                            x_max_value[i] = x
                    elif lab_list[i] != rotated_image[x, y - 1]:
                        if y < y_min_value[i]:
                            y_min_value[i] = y
                    elif lab_list[i] != rotated_image[x, y + 1]:
                        if y > y_max_value[i]:
                            y_max_value[i] = y

        tem_area = abs(x_max_value[i] - x_min_value[i]) * abs(y_max_value[i] - y_min_value[i])
        print("不同角度对应的面积", tem_area)

        if tem_area < area[i]:
            area[i] = tem_area
            area_min_angle[i] = rotated_angle

            min_rect_long_up[i] = x_max_value[i] - tem_image_size[0] / 2
            min_rect_long_down[i] = x_min_value[i] - tem_image_size[0] / 2
            min_rect_width_up[i] = y_max_value[i] - tem_image_size[1] / 2
            min_rect_width_down[i] = y_min_value[i] - tem_image_size[1] / 2

print("绘制边界框中... ...")

tem_a = np.zeros(length, int)
tem_b = np.zeros(length, int)
tem_c = np.zeros(length, int)
tem_r = np.zeros(length, int)
tem_area = np.zeros(length, int)
angle_min = np.zeros(length, float)
angle_max = np.zeros(length, float)

x2_min = np.zeros(length, float)
x2_max = np.zeros(length, float)

tem_test = 0

# 计算 a b c 距离求和
for i in range(length):
    print("边界框绘制进度：", i / length * 100, "%")

    if avg_x[i] < 0:
        continue

    for x in range(0, image_size[0]):
        for y in range(0, image_size[1]):
            if tem_image[x, y] == lab_list[i]:
                tem_c[i] += (x - avg_x[i]) ** 2
                tem_b[i] += 2 * (x - avg_x[i]) * (y - avg_y[i])
                tem_a[i] += (y - avg_y[i]) ** 2
                tem_area[i] += 1

    tem_test = tem_b[i] / (tem_a[i] - tem_c[i])

    angle_max[i] = (np.arctan(tem_test) / 2) + 3.14159265357 / 2
    angle_min[i] = (np.arctan(tem_test) / 2)

print("绘制椭圆中... ...")

for i in range(length):
    if avg_x[i] < 0 or avg_y[i] < 0:
        continue
    draw_rec((avg_y[i], avg_x[i]), abs(min_rect_width_down[i]), abs(min_rect_width_up[i]),
             abs(min_rect_long_up[i]), abs(min_rect_long_down[i]), area_min_angle[i])

k = np.zeros(length, float)
kk = np.zeros(length, float)
point_1 = (0, 0)
point_2 = (0, 0)

for i in range(length):
    if avg_x[i] < 0 or avg_y[i] < 0:
        continue

    k[i] = np.tan((3.14159265354 / 2) - angle_min[i])

    if math.isnan(k[i]) or k[i] > 9999999:
        k[i] = 9999999
        kk[i] = 0
    else:
        kk[i] = (-1) / k[i]
    # 斜率不存在的情况

x2_min = np.zeros(length, float)
x2_max = np.zeros(length, float)
E = np.zeros(length, float)

# 计算转动惯量的最值
for i in range(length):
    print("计算转动惯量的最值进度：", i / length * 100, "%")

    if avg_x[i] < 0 or avg_y[i] < 0:
        continue

    line1 = k[i], avg_y[i] - k[i] * avg_x[i]
    line2 = kk[i], avg_y[i] - kk[i] * avg_x[i]

    for x in range(0, image_size[0]):
        for y in range(0, image_size[1]):
            if tem_image[x, y] == lab_list[i]:
                distance = get_distance_point2line((x, y), line1)
                x2_min[i] += distance
                distance = get_distance_point2line((x, y), line2)
                x2_max[i] += distance

# 计算出正圆率
for i in range(length):
    # 防止分母为0
    E[i] = x2_min[i] / x2_max[i]
    if x2_max[i] == 0:
        E[i] = 1

print("计算椭圆的长轴中... ...")

tem_a = tem_a / tem_area
tem_b = tem_b / tem_area
tem_c = tem_c / tem_area

long_dis = np.zeros(length, float)
short_dis = np.zeros(length, float)

for i in range(length):
    if avg_x[i] < 0 or avg_y[i] < 0:
        continue

    long_dis[i] = np.sqrt(2 * (tem_c[i] + tem_a[i] + np.sqrt((tem_c[i] - tem_a[i]) ** 2 + 4 * (tem_b[i] ** 2))))
    short_dis[i] = long_dis[i] * E[i]

    center_coordinates = (int(avg_y[i]), int(avg_x[i]))
    axesLength = (int(long_dis[i]), int(short_dis[i]))
    angle = 180 * (angle_min[i]) / np.pi
    startAngle = 0
    endAngle = 360
    color = (0, 255, 255)
    thickness = 2
    cv2.ellipse(image, center_coordinates, axesLength, angle, startAngle, endAngle, color, thickness)

    point_1 = (int(k[i] * (long_dis[i] * np.cos(np.arctan(k[i]))) + avg_y[i]),
               int(avg_x[i] + long_dis[i] * np.cos(np.arctan(k[i]))))
    point_2 = (int(k[i] * ((-1) * long_dis[i] * np.cos(np.arctan(k[i]))) + avg_y[i]),
               int(avg_x[i] - long_dis[i] * np.cos(np.arctan(k[i]))))

    cv2.line(image, point_2, point_1, (0, 255, 0), 2)

    point_1 = (int(kk[i] * (short_dis[i] * np.cos(np.arctan(kk[i]))) + avg_y[i]),
               int(avg_x[i] + short_dis[i] * np.cos(np.arctan(kk[i]))))
    point_2 = (int(kk[i] * ((-1) * short_dis[i] * np.cos(np.arctan(kk[i]))) + avg_y[i]),
               int(avg_x[i] - short_dis[i] * np.cos(np.arctan(kk[i]))))
    cv2.line(image, point_2, point_1, (0, 255, 0), 2)

# 输出
output = np.hstack((output, image))

height, width, _ = image.shape
cv2.namedWindow('Output Image', cv2.WINDOW_NORMAL)
cv2.resizeWindow('Output Image', width, height)
cv2.imshow('Output Image', output)
cv2.imwrite('Result_image/Output.png', output)
cv2.waitKey(0)
cv2.destroyAllWindows()

print("图像大小：", image_size)
print("几何中心横坐标：", avg_x)
print("几何中心纵坐标：", avg_y)
print("联通体数：", lab_list)
print("离心率：", E)
print("椭圆长轴长度：", long_dis)
print("椭圆短轴长度：", short_dis)
print("矩形面积：", area)
print("area_min_angle", area_min_angle)
