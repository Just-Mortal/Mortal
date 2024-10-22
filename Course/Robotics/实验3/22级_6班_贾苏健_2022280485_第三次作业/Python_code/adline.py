# TIME : 2023/12/19 15:08

# 自己编写 adline 感知器，区分数字 6

"""
提交前说明：2023.12.26 00：56
1、此文件的训练函数是根据老师上课讲的 adaline 计算公式编写的
2、从官网下载的数据集其是包含训练部分和测试部分，但为了满足实验要求，我对官网的测试集进行了划分。
利用官网测试集的 80% 训练，利用剩余 20% 测试
3、关于测试集绘制相应的函数，是根据网上资料了解到二元分类常见的图标有 ROC 和 混淆矩阵
绘制图像也是自己根据原理编写的。
4、关于训练的停止准则，我最初给较小的阈值，得出大概稳定时的阈值，并将它设置为最终的阈值。本实验约0.018
5、本文件时间复杂度较高，如果运行起来时间较长可以直接看 adline_2.ipynb
6、其他见实验报告
"""

import numpy as np
import matplotlib.pyplot as plt


# -------------------- 自定义函数 begin -------------------- #
# Try 2 begin -------------------------------------------------- #
def load_mnist_images(filename):
    with open(filename, 'rb') as f:
        data = np.fromfile(f, dtype=np.uint8, count=-1)
    return data[16:].reshape(-1, 28*28) / 255.0


def load_mnist_labels(filename):
    with open(filename, 'rb') as f:
        data = np.fromfile(f, dtype=np.uint8, count=-1)
    return data[8:]


def split_dataset(x, y, test_size=0.2, random_state=None):
    # 获取数据集的长度
    data_size = len(x)

    # 根据比例计算验证集的大小
    val_size = int(test_size * data_size)

    # 设置随机种子
    if random_state is not None:
        np.random.seed(random_state)

    # 随机打乱数据集
    indices = np.random.permutation(data_size)

    # 划分数据集
    val_indices = indices[:val_size]
    train_indices = indices[val_size:]

    # 获取划分后的数据
    x_train, x_val = x[train_indices], x[val_indices]
    y_train, y_val = y[train_indices], y[val_indices]

    return x_train, x_val, y_train, y_val


# 初始化权重
def initialize_weights(input_variables_size, weights_init_val=None):
    if weights_init_val is not None:
        weight_i = np.ones(input_variables_size) * weights_init_val
        weight_0 = np.ones(1) * weights_init_val
    else:
        weight_i = np.random.rand(input_variables_size)
        weight_0 = np.random.rand(1)
    return weight_i, weight_0


def adaline_train(x, y, weights_init_val=None, learning_rate=0.01, epochs=100, stopping_threshold=0.018):
    input_variables_size = len(x[0])
    weight_i, weight_0 = initialize_weights(input_variables_size, weights_init_val)

    errors = []

    for epoch in range(epochs):
        error = 0
        for xi, target in zip(x, y):
            dot_product = sum(xi_j * weight_i_j for xi_j, weight_i_j in zip(xi, weight_i))
            prediction = dot_product + weight_0
            update = learning_rate * (target - prediction)
            weight_i += update * xi
            weight_0 += update
            error += (target - prediction) ** 2  # 使用平方损失函数

        mean_squared_error = error / len(y)
        errors.append(mean_squared_error)

        if mean_squared_error < stopping_threshold:
            break

    return errors, weight_i, weight_0


# 使用训练好的参数进行预测
def predict_with_parameters(x, weight_i, weight_0):
    predictions = np.sign(np.dot(x, weight_i) + weight_0)
    return predictions


# 计算错误率
def calculate_error_rate(predictions, actual_labels):
    incorrect_samples = np.sum(predictions != actual_labels)
    error_rate = incorrect_samples / len(actual_labels)
    return error_rate


# 计算 roc 参数正真比例与假正比例
def calculate_roc_rate(verify, predict):
    true_positive = 0
    false_positive = 0
    true_negative = 0
    false_negative = 0

    for xi in range(len(verify)):
        if predict(xi) == 1:
            if verify(xi) == 1:
                true_positive += 1
            else:
                false_positive += 1
        else:
            if verify(xi) == -1:
                true_negative += 1
            else:
                false_negative += 1

# 2023.12.26 考古，有更简单的实现方式且可以避免循环减少时间复杂度：
#     true_positive = np.sum((verify == 1) & (predict == 1))
#     false_positive = np.sum((verify == -1) & (predict == 1))
#     true_negative = np.sum((verify == -1) & (predict == -1))
#     false_negative = np.sum((verify == 1) & (predict == -1))

    true_positive_rate = true_positive / (true_positive + false_negative)
    false_negative_rate = false_positive / (false_positive + true_negative)

    return true_positive_rate, false_negative_rate


# 计算混淆矩阵
def calculate_confusion_matrix(verify, predict):
    true_positive = np.sum((verify == 1) & (predict == 1))
    false_positive = np.sum((verify == -1) & (predict == 1))
    true_negative = np.sum((verify == -1) & (predict == -1))
    false_negative = np.sum((verify == 1) & (predict == -1))

    return np.array([[true_negative, false_positive], [false_negative, true_positive]])


# 绘制混淆矩阵
def plot_confusion_matrix(confusion_matrix, title):
    plt.imshow(confusion_matrix, interpolation='nearest', cmap='viridis')
    plt.title(title)
    plt.colorbar()
    classes = ['other', '6']
    tick_marks = np.arange(len(classes))
    plt.xticks(tick_marks, classes)
    plt.yticks(tick_marks, classes)
    plt.xlabel('true label')
    plt.ylabel('predict label')
# -------------------- 自定义函数 end  -------------------- #


train_images_file = 'train_data/train-images.idx3-ubyte'
train_labels_file = 'train_data/train-labels.idx1-ubyte'

train_images_array = load_mnist_images(train_images_file)
train_labels_array = load_mnist_labels(train_labels_file)

train_label_binary = np.where(train_labels_array == 6, 1, -1)

# 划分训练集和验证集
train_feature, verify_feature, train_label, verify_label = \
    split_dataset(train_images_array, train_label_binary, test_size=0.2, random_state=42)

# ---------- test code begin ---------- #
# print(len(train_feature[0]))
# print(len(train_label))
# ---------- test code end   ---------- #

# Try 2 end -------------------------------------------------- #

# 实验1: 不同初始权重
errors1, final_weight_i_1, final_weight_0_1 = \
    adaline_train(train_feature, train_label, weights_init_val=0, learning_rate=0.01, stopping_threshold=0.018)
errors2, final_weight_i_2, final_weight_0_2 = \
    adaline_train(train_feature, train_label, weights_init_val=1, learning_rate=0.01, stopping_threshold=0.018)

# 实验2: 不同训练速率
errors3, final_weight_i_3, final_weight_0_3 = \
    adaline_train(train_feature, train_label, weights_init_val=1, learning_rate=0.001, stopping_threshold=0.018)
errors4, final_weight_i_4, final_weight_0_4 = \
    adaline_train(train_feature, train_label, weights_init_val=1, learning_rate=0.08, stopping_threshold=0.018)

# 绘制训练集的错误率随时间变化的图表
plt.figure(figsize=(12, 8))
plt.subplot(2, 2, 1)
plt.plot(range(1, len(errors1) + 1), errors1, marker='o')
plt.xlabel('Epochs')
plt.ylabel('% Incorrect Classification')
plt.title('Experiment 1: Zero Initial Weights - Learning Rate: 0.01')

plt.subplot(2, 2, 2)
plt.plot(range(1, len(errors2) + 1), errors2, marker='o')
plt.xlabel('Epochs')
plt.ylabel('% Incorrect Classification')
plt.title('Experiment 1: Ones Initial Weights - Learning Rate: 0.01')

plt.subplot(2, 2, 3)
plt.plot(range(1, len(errors3) + 1), errors3, marker='o')
plt.xlabel('Epochs')
plt.ylabel('% Incorrect Classification')
plt.title('Experiment 2: Zero Initial Weights - Learning Rate: 0.001')

plt.subplot(2, 2, 4)
plt.plot(range(1, len(errors4) + 1), errors4, marker='o')
plt.xlabel('Epochs')
plt.ylabel('% Incorrect Classification')
plt.title('Experiment 2: Zero Initial Weights - Learning Rate: 0.08')

# -------------------- 为测试集绘制相应的图表 begin -------------------- #
# 在测试集上验证不同实验的错误率
predict_label_1 = predict_with_parameters(verify_feature, final_weight_i_1, final_weight_0_1)
predict_label_2 = predict_with_parameters(verify_feature, final_weight_i_2, final_weight_0_2)
predict_label_3 = predict_with_parameters(verify_feature, final_weight_i_3, final_weight_0_3)
predict_label_4 = predict_with_parameters(verify_feature, final_weight_i_4, final_weight_0_4)
error_rate_1 = calculate_error_rate(predict_label_1, verify_label)
error_rate_2 = calculate_error_rate(predict_label_2, verify_label)
error_rate_3 = calculate_error_rate(predict_label_3, verify_label)
error_rate_4 = calculate_error_rate(predict_label_4, verify_label)

print("Experiment 1 Error Rate (Zero Initial Weights):", error_rate_1)
print("Experiment 1 Error Rate (Ones Initial Weights):", error_rate_2)
print("Experiment 2 Error Rate (Learning Rate: 0.001):", error_rate_3)
print("Experiment 2 Error Rate (Learning Rate: 0.08):", error_rate_4)

# -------------------- test code begin -------------------- #
# from sklearn.metrics import classification_report
# from sklearn.metrics import confusion_matrix
# import seaborn as sns
# from sklearn.metrics import roc_curve, auc

# predictions_verify = predict_with_parameters(verify_feature, final_weight_i, final_weight_0)
#
# print(classification_report(verify_label, predictions_verify))
#
# cm = confusion_matrix(verify_label, predictions_verify)
# sns.heatmap(cm, annot=True, fmt='d', cmap='Blues', cbar=False)
# plt.title('Confusion Matrix')
# plt.xlabel('Predicted Labels')
# plt.ylabel('True Labels')
# plt.show()

# Assuming verify_feature is your test set features and verify_label is the true labels
# Assuming adaline_activation is your ADALINE activation function
# verify_predictions = [np.dot(xi, final_weight_i) + final_weight_0 for xi in verify_feature]
#
# # Calculate ROC curve and AUC
# fpr, tpr, thresholds = roc_curve(verify_label, verify_predictions)
# roc_auc = auc(fpr, tpr)
#
# # Plot ROC Curve
# plt.figure(figsize=(8, 6))
# plt.plot(fpr, tpr, color='darkorange', lw=2, label='ROC curve (area = {:.2f})'.format(roc_auc))
# plt.plot([0, 1], [0, 1], color='navy', lw=2, linestyle='--')
# plt.xlabel('False Positive Rate')
# plt.ylabel('True Positive Rate')
# plt.title('Receiver Operating Characteristic (ROC) Curve')
# plt.legend(loc='lower right')
# plt.show()
# -------------------- test code end   -------------------- #

# -------------------- 为测试集绘制ROC曲线 begin -------------------- #

# -------------------- test code begin -------------------- #
# print(len(predict_label_1))
# print(predict_label_1(5))
# -------------------- test code end   -------------------- #

# 计算测试集的真正例率（True Positive Rate）和假正例率（False Positive Rate）
# ---------- 未封装的代码 begin ---------- #
# true_positive_1 = 0
# false_positive_1 = 0
# true_negative_1 = 0
# false_negative_1 = 0
# for index in range(len(predict_label_1)):
#     if predict_label_1(index) == 1:
#         if verify_label(index) == 1:
#             true_positive_1 += 1
#         else:
#             false_positive_1 += 1
#     else :
#         if verify_label(index) == -1:
#             true_negative_1 += 1
#         else:
#             false_negative_1 += 1
#
# positive_rate_1 = true_positive_1 / (true_positive_1 + false_negative_1)
# negative_rate_1 = false_positive_1 / (false_positive_1 + true_negative_1)
# ---------- 未封装的代码 end  ---------- #

tpr_1, fpr_1 = calculate_roc_rate(verify_label, predict_label_1)
tpr_2, fpr_2 = calculate_roc_rate(verify_label, predict_label_2)
tpr_3, fpr_3 = calculate_roc_rate(verify_label, predict_label_3)
tpr_4, fpr_4 = calculate_roc_rate(verify_label, predict_label_4)

# 绘制 ROC 曲线
plt.figure(figsize=(12, 8))

plt.subplot(2, 2, 1)
plt.plot(tpr_1, fpr_1, color='darkorange', lw=2)
plt.plot([0, 1], [0, 1], color='navy', lw=2, linestyle='--')
plt.xlabel('False Positive Rate')
plt.ylabel('True Positive Rate')
plt.title('Experiment 1: Zero Initial Weights - Learning Rate: 0.01')

plt.subplot(2, 2, 2)
plt.plot(tpr_2, fpr_2, color='green', lw=2)
plt.plot([0, 1], [0, 1], color='navy', lw=2, linestyle='--')
plt.xlabel('False Positive Rate')
plt.ylabel('True Positive Rate')
plt.title('Experiment 1: Ones Initial Weights - Learning Rate: 0.01')

plt.subplot(2, 2, 3)
plt.plot(tpr_3, fpr_3, color='blue', lw=2)
plt.plot([0, 1], [0, 1], color='navy', lw=2, linestyle='--')
plt.xlabel('False Positive Rate')
plt.ylabel('True Positive Rate')
plt.title('Experiment 2: Zero Initial Weights - Learning Rate: 0.001')

plt.subplot(2, 2, 4)
plt.plot(tpr_4, fpr_4, color='red', lw=2)
plt.plot([0, 1], [0, 1], color='navy', lw=2, linestyle='--')
plt.xlabel('False Positive Rate')
plt.title('Experiment 2: Zero Initial Weights - Learning Rate: 0.08')

plt.tight_layout()  # 调整布局以防止重叠
plt.show()

# -------------------- 为测试集绘制ROC曲线 end -------------------- #

# -------------------- 为测试集绘制混淆矩阵 begin -------------------- #
# 计算每个实验的混淆矩阵
confusion_matrix_1 = calculate_confusion_matrix(verify_label, predict_label_1)
confusion_matrix_2 = calculate_confusion_matrix(verify_label, predict_label_2)
confusion_matrix_3 = calculate_confusion_matrix(verify_label, predict_label_3)
confusion_matrix_4 = calculate_confusion_matrix(verify_label, predict_label_4)

# Plot confusion matrix for each experiment
plt.figure(figsize=(12, 10))

plt.subplot(2, 2, 1)
plot_confusion_matrix(confusion_matrix_1, 'Experiment 1: Zero Initial Weights - Learning Rate: 0.01')

plt.subplot(2, 2, 2)
plot_confusion_matrix(confusion_matrix_2, 'Experiment 1: Ones Initial Weights - Learning Rate: 0.01')

plt.subplot(2, 2, 3)
plot_confusion_matrix(confusion_matrix_3, 'Experiment 2: Zero Initial Weights - Learning Rate: 0.001')

plt.subplot(2, 2, 4)
plot_confusion_matrix(confusion_matrix_4, 'Experiment 2: Zero Initial Weights - Learning Rate: 0.08')

plt.tight_layout()
plt.show()
# -------------------- 为测试集绘制混淆矩阵 end   -------------------- #
# -------------------- 为测试集绘制相应的图表 end   -------------------- #
