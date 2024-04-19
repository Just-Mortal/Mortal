老师/助教，你好。此 txt 文件用于介绍此文件夹。
此文件夹包含三个子文件夹 Report、Python_code 、Matlab_code。

-Report
--机器人导论实验报告-3-贾苏健-中文.docx	最终提交英文实验报告大部分是软件翻译的，因为担心有词不达意的地方，所以提交最初的中文版实验报告
--机器人导论实验报告-3-贾苏健-中文.pdf 	防止乱码，提交 pdf 格式
--机器人导论实验报告-3-贾苏健.docx		最终提交实验报告版本
--机器人导论实验报告-3-贾苏健.pdf		防止乱码，提交 pdf 格式
--区分0-9的MLP.xmind 			区分数字0-9的完成思路，实验报告有用到
--多层感知器.xmind 			完成实验过程中我的一些理解


-Python_code
--test_data			从MNIST网站下载的测试集数据集
--train_data			从MNIST网站下载的训练集数据集
--Demo2.ipynb			草稿（包括读取，展示，划分图像部分）
--Single_perceptron.py		符号函数作为损失函数的感知器
--Single_perceptron.ipynb	包含程序运行结果
--adaline.py			自适应训练，均方差值作为损失函数
--adaline.ipynb			包含程序运行结果
--additional_question.py	分类数字0-9
--additional_question.py	包含程序运行结果
（注：因为 .py 程序运行时间过长，所以添加了 .ipynb 文件。二者代码一样， .ipynb 增加了程序运行结果。


-Matlab_code
--test_data 从MNIST网站下载的测试集数据集
--train_data 从MNIST网站下载的训练集数据集

（函数文件）
--load_mnist_images.m		% 读取数据集的图像
--load_mnist_labels.m		% 读取数据集的标签
--initialize_weights.m		% 初始化感知器的权重
--single_perceptron.m		% 符号函数作为损失函数
--adaline_train.m		% 均方差值作为损失函数
--displayMisclassifiedImages.m	% 展示被错误分类的图像

（脚本文件）
--show_imshow.m			% 展示前十张图片，验证数据读取
--data_load.m			% 加载数据集图像及标签
--Single_perceptron_train.m	% 符号函数作为损失函数的训练，区分数字 6 和其他数字
--adaline.m			% 均方差值作为损失函数的训练，区分数字 6 和其他数字
--task1_SLP.m			% matlab实现区分数字2和数字5
--task2_MLP.m			% matlab实现区分数字1、2、5、7
--task3.m			% matlab实现区分数字 0 - 9

--temp_function
---calculateSampleCount.m	% 计算数据集中各标签的数量（未在脚本文件引用，可直接调用调试）
---displayDigitSamples.m	% 展示特定数字的图像（未在脚本文件引用，可直接调用调试）
