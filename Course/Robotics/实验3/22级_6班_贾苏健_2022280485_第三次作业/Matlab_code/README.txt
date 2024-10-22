matlab 文件夹文件说明
函数文件：
% 读取数据集的图像
load_mnist_images.m
% 读取数据集的标签
load_mnist_labels.m
% 初始化感知器的权重
initialize_weights.m
% 符号函数作为损失函数
single_perceptron.m
% 均方差值作为损失函数
adaline_train.m
% 展示被错误分类的图像
displayMisclassifiedImages.m

脚本文件：
% 展示前十张图片，验证数据读取
show_imshow.m
% 加载数据集图像及标签
data_load.m
% 符号函数作为损失函数的训练
Single_perceptron_train.m
% 均方差值作为损失函数的训练
adaline.m
% matlab实现区分数字2和数字5
task1_SLP.m
% matlab实现区分数字1、2、5、7
task2_MLP.m
% matlab实现区分数字 0 - 9
task3.m

以下是子文件夹：
-temp_function
--% 计算数据集中各标签的数量（未在脚本文件引用，可直接调用调试）
--calculateSampleCount.m
--% 展示特定数字的图像（未在脚本文件引用，可直接调用调试）
--displayDigitSamples.m

-test_data 从MNIST网站下载的测试集数据集
-train_data 从MNIST网站下载的训练集数据集