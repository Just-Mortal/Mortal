% 需要先运行数据集载入脚本

train_label = (train_labels_array == 6) * 2 - 1;
test_label = (test_labels_array == 6) * 2 - 1;

% 实验1: 不同初始权重
[errors1, final_weight_i_1, final_weight_0_1] = single_perceptron_train(train_images_array, train_label, 0, 0.01, 100, 0.003);
[errors2, final_weight_i_2, final_weight_0_2] = single_perceptron_train(train_images_array, train_label, 1, 0.01, 100, 0.003);

% 实验2: 不同训练速率
[errors3, final_weight_i_3, final_weight_0_3] = single_perceptron_train(train_images_array, train_label, 1, 0.001, 100, 0.003);
[errors4, final_weight_i_4, final_weight_0_4] = single_perceptron_train(train_images_array, train_label, 1, 0.08, 100, 0.003);

% 绘制训练集的错误率随时间变化的图表
figure;

subplot(2, 2, 1);
plot(1:length(errors1), errors1, '-');
xlabel('Epochs');
ylabel('% Incorrect Classification');
title('Experiment 1: Zero Initial Weights - Learning Rate: 0.01');

subplot(2, 2, 2);
plot(1:length(errors2), errors2, '-');
xlabel('Epochs');
ylabel('% Incorrect Classification');
title('Experiment 1: Ones Initial Weights - Learning Rate: 0.01');

subplot(2, 2, 3);
plot(1:length(errors3), errors3, '-');
xlabel('Epochs');
ylabel('% Incorrect Classification');
title('Experiment 2: Zero Initial Weights - Learning Rate: 0.001');

subplot(2, 2, 4);
plot(1:length(errors4), errors4, '-');
xlabel('Epochs');
ylabel('% Incorrect Classification');
title('Experiment 2: Zero Initial Weights - Learning Rate: 0.08');
