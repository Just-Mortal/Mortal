import cv2 as cv
from matplotlib import pyplot as plt
from scipy import signal
import numpy as np
import math


no_value = 200
x_times = 1
y_times = 1
min_tre = 50000
fli_win = 2
correct_f = 50
text_img = cv.imread(r"images\box3.jpg", cv.IMREAD_COLOR)
img_copy = cv.imread(r"images\box3.jpg", cv.IMREAD_COLOR)

#
# text_img = cv.imread(r"images\test.jpg", cv.IMREAD_COLOR)
# img_copy = cv.imread(r"images\test.jpg", cv.IMREAD_COLOR)





"""中值滤波"""
def Median_blur(image, win_size):
    median_img = np.copy(image)
    for h in range(0, median_img.shape[0] - win_size + 1):
        for w in range(0, median_img.shape[1] - win_size + 1):
            median_img[h, w] = np.median(median_img[h:h + win_size, w:w + win_size])

    return median_img

"""获取欧氏距离"""
def get_distance(x1, y1, x2, y2):
    return np.sqrt(np.power(x1 - x2, 2) + np.power(y1 - y2, 2))

"""点到线距离计算公式"""
def get_distance_point2line(point, line_ab):
    """
    Args:
        point: [x0, y0]
        line_ab: [k, b]
    """
    k_line, b = line_ab
    distance = abs(k_line * point[0] - point[1] + b) / math.sqrt(k_line ** 2 + 1)
    return distance


class Point(object):
    def __init__(self, x, y):
        self.x = x
        self.y = y

    def getX(self):
        return self.x

    def getY(self):
        return self.y


"""线的颜色"""
line_color = (0, 255, 0)

"""线的厚度"""
line_thickness = 2

"""绘制旋转矩形"""

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

    cv.line(text_img, (int(x1), int(y1)), (int(x2), int(y2)), (144, 144, 0), thickness=2)
    cv.line(text_img, (int(x2), int(y2)), (int(x3), int(y3)), (144, 144, 0), thickness=2)
    cv.line(text_img, (int(x3), int(y3)), (int(x4), int(y4)), (144, 144, 0), thickness=2)
    cv.line(text_img, (int(x4), int(y4)), (int(x1), int(y1)), (144, 144, 0), thickness=2)


# 图像边缘不能有东西
# 8 9
# 5 7 别用

img_size = np.shape(text_img)[0:2]

# 尝试分区设置阈值
# 先将读入的图片转灰度
chanel_B = text_img[:, :, 0]
chanel_G = text_img[:, :, 1]
chanel_R = text_img[:, :, 2]

img_gray = 0.114 * chanel_B + 0.299 * chanel_G + 0.587 * chanel_R

# 自适应的阈值转换 从整张图片的像素来进行区分 会出现一个波谷，那就是阈值的取值点
# 统计图片中每个像素出现的次数

# 考虑没有极小值 与 极小值不止一个的情况
for x_domain in range(x_times):
    for y_domain in range(y_times):
        BGR_y_space = [0] * 256
        BGR_X_space = np.arange(0, 256)

        for x in range(int((img_size[0] / x_times) * x_domain), int((img_size[0] / x_times) * (x_domain + 1))):
            for y in range(int((img_size[1] / y_times) * y_domain), int((img_size[1] / y_times) * (y_domain + 1))):
                # img_gray[x, y] = int(img_gray[x, y])
                BGR_y_space[int(img_gray[x, y])] += 1

        BGR_y_space = np.asanyarray(BGR_y_space, dtype=np.int32)

        # 先要寻找波峰(通过拟合曲线)
        z1 = np.polyfit(BGR_X_space, BGR_y_space, 5)
        p1 = np.poly1d(z1)
        BGR_fit_space = p1(BGR_X_space)
        plt.plot(BGR_X_space, BGR_y_space, '*', label='original values')
        plt.plot(BGR_X_space, BGR_fit_space, 'r', label='polyfit values')

        # # 寻找拟合曲线的最小值
        plt.plot(signal.argrelextrema(BGR_fit_space, np.less)[0],
                 BGR_fit_space[signal.argrelextrema(BGR_fit_space, np.less)],
                 '.', markersize=10)
        # plt.show()
        # 得到自适应的阈值
        less_point = signal.argrelextrema(BGR_fit_space, np.less)[0]
        print(less_point)
        if less_point.size == 0:
            less_point = [no_value]

        bmp_flag = min(less_point) + correct_f

        print(bmp_flag)
        # 转bmp
        for i in range(int((img_size[0] / x_times) * x_domain), int((img_size[0] / x_times) * (x_domain + 1))):
            for j in range(int((img_size[1] / y_times) * y_domain), int((img_size[1] / y_times) * (y_domain + 1))):
                if img_gray[i, j] >= bmp_flag:
                    img_gray[i, j] = 255
                else:
                    img_gray[i, j] = 0

# 中值滤波
img_gray = Median_blur(img_gray, fli_win)

white = 0
black = 255
tem_img = np.zeros(img_size)
label = 1
flag = 0

object_num = 0
object_lab = 0
same_lab = 0

for x in range(1, img_size[0] - 1):
    for y in range(1, img_size[1] - 1):
        # case 1
        print(x, y)
        if img_gray[x, y] == black:
            tem_img[x, y] = black
            if flag == 1:
                label += 1
                flag = 0
                if label == 255:
                    label += 1
        else:
            # case 2
            if img_gray[x - 1, y] == black and img_gray[x, y - 1] == black and img_gray[x - 1, y - 1] == black:
                tem_img[x, y] = label
                flag = 1
                same_lab = 0
            # case3
            elif img_gray[x - 1, y - 1] != black:
                tem_img[x, y] = tem_img[x - 1, y - 1]
                flag = 1
            # case4
            elif img_gray[x - 1, y] == black and img_gray[x - 1, y - 1] == black and img_gray[x, y - 1] != black:
                tem_img[x, y] = tem_img[x, y - 1]
                flag = 1
            # case5
            elif img_gray[x, y - 1] == black and img_gray[x - 1, y - 1] == black and img_gray[x - 1, y] != black:
                tem_img[x, y] = tem_img[x - 1, y]
                flag = 1
            # case6
            elif img_gray[x - 1, y] != black and img_gray[x - 1, y - 1] == black and img_gray[x, y - 1] != black:
                tem_img[x, y] = tem_img[x, y - 1]
                flag = 1

for x in range(0, img_size[0]):
    for y in range(0, img_size[1]):
        if tem_img[x, y] == 0:
            tem_img[x, y] = 255


connects = [Point(-1, -1), Point(0, -1), Point(1, -1), Point(1, 0), Point(1, 1), Point(0, 1), Point(-1, 1),
            Point(-1, 0)]
img_mark = np.zeros(img_size)
seed_list = []
lab_list = []
for x in range(1, img_size[0] - 1):
    for y in range(1, img_size[1] - 1):
        # img_mark[x, y] = 1
        if tem_img[x, y] != 255 and img_mark[x, y] == 0:
            # img_mark[x, y] = 1
            seed_list.append(Point(x, y))
            while len(seed_list) > 0:
                seed_tmp = seed_list[0]
                seed_list.pop(0)
                for i in range(8):
                    tmpX = seed_tmp.x + connects[i].x
                    tmpY = seed_tmp.y + connects[i].y
                    tmpLabel = int(tem_img[x, y])
                    if img_mark[tmpX, tmpY] != 1 and tem_img[tmpX, tmpY] != black and tem_img[tmpX, tmpY] != white:
                        tem_img[tmpX, tmpY] = tmpLabel
                        img_mark[tmpX, tmpY] = 1
                        seed_list.append(Point(tmpX, tmpY))
                        if tmpLabel not in lab_list:
                            lab_list.append(tmpLabel)

B = 0
G = 155
R = 255

# 画几何中心
length = len(lab_list)
avg_x = np.zeros(length, int)
avg_y = np.zeros(length, int)
avg_x_cnt = np.zeros(length, int)
avg_y_cnt = np.zeros(length, int)
a_pre = 0
b_pre = 0
c_pre = 0

for x in range(0, img_size[0]):
    for y in range(0, img_size[1]):
        if tem_img[x, y] in lab_list:
            avg_x[lab_list.index(tem_img[x, y])] += x
            avg_x_cnt[lab_list.index(tem_img[x, y])] += 1

            avg_y[lab_list.index(tem_img[x, y])] += y
            avg_y_cnt[lab_list.index(tem_img[x, y])] += 1

            a_pre += x ** 2

for i in range(length):
    if avg_x[i] < min_tre or avg_y[i] < min_tre:
        avg_x[i] = -5
        avg_y[i] = -5

avg_x = avg_x / avg_x_cnt
avg_y = avg_y / avg_y_cnt

avg_x = avg_x.tolist()
avg_y = avg_y.tolist()

for i in range(length):
    tmpX = int(avg_x[i])
    tmpY = int(avg_y[i])
    text_img[tmpX, tmpY, 0] = 0
    text_img[tmpX, tmpY, 1] = 0
    text_img[tmpX, tmpY, 2] = 255
    for j in range(8):
        for k in range(8):
            tmpX = int(avg_x[i]) + connects[j].x + connects[k].x
            tmpY = int(avg_y[i]) + connects[j].y + connects[k].y
            if tmpX < 0 or tmpY < 0 or tmpX >= img_size[0] or tmpY >= img_size[1]:
                continue
            text_img[tmpX, tmpY, 0] = 0
            text_img[tmpX, tmpY, 1] = 0
            text_img[tmpX, tmpY, 2] = 255

new_list = np.zeros(length, int)
new_list = new_list.tolist()
tem_cnt = 0
for i in range(length):
    if avg_x[i] > 0 and avg_y[i] > 0:
        new_list[tem_cnt] = lab_list[i]
        tem_cnt += 1
        
    
# 画边界
for x in range(0, img_size[0]):
    for y in range(0, img_size[1]):

        if tem_img[x, y] != 255 and tem_img[x, y] in new_list:
            if tem_img[x - 1, y] != tem_img[x, y] or tem_img[x + 1, y] != tem_img[x, y] or tem_img[x, y - 1] != tem_img[
                x, y] or tem_img[x, y + 1] != tem_img[x, y]:
                text_img[x, y, 0] = B
                text_img[x, y, 1] = G
                text_img[x, y, 2] = R

                text_img[x + 1, y, 0] = B
                text_img[x + 1, y, 1] = G
                text_img[x + 1, y, 2] = R

                text_img[x - 1, y, 0] = B
                text_img[x - 1, y, 1] = G
                text_img[x - 1, y, 2] = R

                text_img[x, y + 1, 0] = B
                text_img[x, y + 1, 1] = G
                text_img[x, y + 1, 2] = R

output = np.hstack((img_copy, text_img))

# 找到最大的点然后画框 上下左右四个点
y_max = [[0 for i in range(img_size[0])] for i in range(img_size[1])]
y_min = [[0 for i in range(img_size[0])] for i in range(img_size[1])]
x_min = [[0 for i in range(img_size[0])] for i in range(img_size[1])]
x_max = [[0 for i in range(img_size[0])] for i in range(img_size[1])]

x_min_value = np.zeros(length, int)
x_max_value = np.zeros(length, int)
y_min_value = np.zeros(length, int)
y_max_value = np.zeros(length, int)

for i in range(length):
    x_min_value[i] = 9999
    y_min_value[i] = 9999
    x_max_value[i] = 0
    y_max_value[i] = 0

tem_cnt = [img_size[0], 0, img_size[1], 0]  # x_min x_max y_min y_max

img_tem_rota = img_copy
area = np.zeros(length, float)
area_min_angle = np.zeros(length, float)
min_rect_long_up = np.zeros(length, float)
min_rect_width_up = np.zeros(length, float)
min_rect_long_down = np.zeros(length, float)
min_rect_width_down = np.zeros(length, float)
tem_area = 0
cnt = 0
for i in range(length):
    if avg_x[i] < 0 or avg_y[i] < 0:
        continue

    area[i] = img_size[0] * img_size[1]
    # center = (avg_y[i], avg_x[i])
    center = (img_size[0], img_size[1])

    # 平移 先将目标平移到屏幕中心，再开始旋转
    ty = (-1) * (avg_x[i] - img_size[1])
    tx = (-1) * (avg_y[i] - img_size[0])

    translation_matrix = np.array([
        [1, 0, tx],
        [0, 1, ty]
    ], dtype=np.float32)
    translation_image = cv.warpAffine(src=tem_img, M=translation_matrix, dsize=(img_size[0] * 2, img_size[1] * 2))
    # 遍历每一个角度
    for rotated_angle in range(0, 90, 10):
        print(rotated_angle)
        # 旋转
        rotate_matrix = cv.getRotationMatrix2D(center=center, angle=rotated_angle, scale=1)
        rotated_image = cv.warpAffine(src=translation_image, M=rotate_matrix, dsize=(img_size[0] * 2, img_size[1] * 2))
        tem_img_size = np.shape(rotated_image)[0:2]
        # 每个角度对应一幅图也要清零
        x_min_value[i] = 9999
        y_min_value[i] = 9999
        x_max_value[i] = 0
        y_max_value[i] = 0

        for x in range(100, tem_img_size[0] - 100):
            for y in range(100, tem_img_size[1] - 100):
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

        # 测试代码
        # cv.rectangle(rotated_image, (y_min_value[i], x_min_value[i]), (y_max_value[i], x_max_value[i]), (144, 144, 0),
        #              2)
        # cv.imshow("rotated_image1", np.int8(rotated_image))
        # cv.waitKey(0)
        # cv.imwrite(f'sample {rotated_angle}',np.int8(rotated_image))
        # 测试代码 end
        tem_area = abs(x_max_value[i] - x_min_value[i]) * abs(y_max_value[i] - y_min_value[i])
        print(tem_area)
        if tem_area < area[i]:
            # 面积其实是没用的变量
            # 还应记录下长和宽
            area[i] = tem_area
            area_min_angle[i] = rotated_angle

            min_rect_long_up[i] = x_max_value[i] - tem_img_size[0] / 2
            min_rect_long_down[i] = x_min_value[i] - tem_img_size[0] / 2
            min_rect_width_up[i] = y_max_value[i] - tem_img_size[1] / 2
            min_rect_width_down[i] = y_min_value[i] - tem_img_size[1] / 2

# 画框
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
    if avg_x[i] < 0:
        continue
    for x in range(0, img_size[0]):
        for y in range(0, img_size[1]):
            if tem_img[x, y] == lab_list[i]:
                tem_c[i] += (x - avg_x[i]) ** 2  # 20
                tem_b[i] += 2 * (x - avg_x[i]) * (y - avg_y[i])  # 11
                tem_a[i] += (y - avg_y[i]) ** 2  # 02
                tem_area[i] += 1

    tem_test = tem_b[i] / (tem_a[i] - tem_c[i])

    angle_max[i] = (np.arctan(tem_test) / 2) + 3.14159265357 / 2
    angle_min[i] = (np.arctan(tem_test) / 2)

# 这是在画矩形框
for i in range(length):
    if avg_x[i] < 0 or avg_y[i] < 0:
        continue
    # draw_rec((avg_y[i], avg_x[i]), min_rect_long_up[i], min_rect_long_down[i],min_rect_width_down[i],min_rect_width_up[i],area_min_angle[i])
    draw_rec((avg_y[i], avg_x[i]), abs(min_rect_width_down[i]), abs(min_rect_width_up[i]), abs(min_rect_long_up[i]),
             abs(min_rect_long_down[i]), area_min_angle[i])

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
    # 考虑斜率不存在的情况

x2_min = np.zeros(length, float)
x2_max = np.zeros(length, float)
E = np.zeros(length, float)
# 计算最大最小的转动惯量，然后计算出正圆率，最后再计算出椭圆的a与b的比值
for i in range(length):
    if avg_x[i] < 0 or avg_y[i] < 0:
        continue
    line1 = k[i], avg_y[i] - k[i] * avg_x[i]
    line2 = kk[i], avg_y[i] - kk[i] * avg_x[i]

    for x in range(0, img_size[0]):
        for y in range(0, img_size[1]):
            if tem_img[x, y] == lab_list[i]:
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

# 接下来要计算椭圆的长轴

tem_a = tem_a/tem_area
tem_b = tem_b/tem_area
tem_c = tem_c/tem_area

long_dis = np.zeros(length, float)
short_dis = np.zeros(length, float)
for i in range(length):
    if avg_x[i] < 0 or avg_y[i] < 0:
        continue

    long_dis[i] = np.sqrt(2*(tem_c[i] + tem_a[i] + np.sqrt((tem_c[i] - tem_a[i] ) ** 2 + 4*(tem_b[i] ** 2))))
    short_dis[i] = long_dis[i] * E[i]

    center_coordinates = (int(avg_y[i]), int(avg_x[i]))
    axesLength = (int(long_dis[i]), int(short_dis[i]))
    angle = 180 * (angle_min[i]) / np.pi
    startAngle = 0
    endAngle = 360
    color = (0, 255, 255)
    thickness = 2
    cv.ellipse(text_img, center_coordinates, axesLength, angle, startAngle, endAngle, color, thickness)

    point_1 = (int(k[i] * (long_dis[i] * np.cos(np.arctan(k[i]))) + avg_y[i]),
               int(avg_x[i] + long_dis[i] * np.cos(np.arctan(k[i]))))
    point_2 = (int(k[i] * ((-1) * long_dis[i] * np.cos(np.arctan(k[i]))) + avg_y[i]),
               int(avg_x[i] - long_dis[i] * np.cos(np.arctan(k[i]))))

    cv.line(text_img, point_2, point_1, (0, 255, 0), 2)

    point_1 = (int(kk[i] * (short_dis[i] * np.cos(np.arctan(kk[i]))) + avg_y[i]),
               int(avg_x[i] + short_dis[i] * np.cos(np.arctan(kk[i]))))
    point_2 = (int(kk[i] * ((-1) * short_dis[i] * np.cos(np.arctan(kk[i]))) + avg_y[i]),
               int(avg_x[i] - short_dis[i] * np.cos(np.arctan(kk[i]))))
    cv.line(text_img, point_2, point_1, (0, 255, 0), 2)

# 输出
output = np.hstack((output, text_img))
# cv.imshow("Gray", img_gray)
cv.imshow("Ans", output)
print("------------------------------------------------")
print("img_size", img_size)
print("------------------------------------------------")
print("avg_x", avg_x)
print("------------------------------------------------")
print("avg_y", avg_y)
print("------------------------------------------------")
print("lab_list", lab_list)
print("------------------------------------------------")
print("angle_max", angle_max)
print("angle_min", angle_min)
print("------------------------------------------------")
print("E", E)
print("------------------------------------------------")
print("long_dis", long_dis)
print("short_dis", short_dis)
print("------------------------------------------------")
print("area", area)
print("area_min_angle", area_min_angle)
print("------------------------------------------------")
print("min_rect_width_down", min_rect_width_down)
print("min_rect_width_up", min_rect_width_up)
print("min_rect_long_up", min_rect_long_up)
print("min_rect_long_down", min_rect_long_down)
print("min_rect_long_up + min_rect_long_down", min_rect_long_up + min_rect_long_down)
print("------------------------------------------------")

cv.waitKey(0)
