% 运行之前需要先载入数据集
data_load

% ------------------- parent perceptron train code begin ------------------- %
% 提取数字 '2'、'5'、'1' 和 '7' 的样本
indices_2571 = find(train_labels_array == 2 | train_labels_array == 5 | train_labels_array == 1 | train_labels_array == 7);

feats = double(train_images_array(indices_2571, :));
labels = double(train_labels_array(indices_2571));

% 测试集
test_feats_2571 = double(test_images_array(test_labels_array == 2 | test_labels_array == 5 | test_labels_array == 1 | test_labels_array == 7, :));
test_labels_2571 = double(test_labels_array(test_labels_array == 2 | test_labels_array == 5 | test_labels_array == 1 | test_labels_array == 7));

% 将 '2' 和 '5' 视为正类（+1），'1' 和 '7' 视为负类（-1）
labels_parent = (labels == 2 | labels == 5) * 2 - 1;

% ---------- test code begin ---------- %
% disp("size of 1257 labels");
% disp(size(labels));             % 24386 * 1
% disp("size of 1257 images");
% disp(size(feats))               % 24386 * 784
% disp("size of labels_parent");
% disp(size(labels_parent))       % 24386 * 1
% ---------- test code end   ---------- %

% 使用 Adaline 学习（父感知器）
[~, weight_i_parent, weight_0_parent] = adaline_train(feats, labels_parent, 0, 0.001, 100, 0.02);

% ---------- test code begin ---------- %
% % 使用 Adaline 预测
% predictions_parent = sum(feats .* weight_i_parent, 2) + weight_0_parent;
% % 预测结果二值化为+1和-1
% predictions_binary = sign(predictions_parent);
%
% % 计算训练集准确率
% train_accuracy = sum(predictions_binary == labels_parent) / numel(labels_parent);
% fprintf('Training Accuracy: %.2f%%\n', train_accuracy * 100);
% 
% % 在测试集上进行预测
% test_predictions_parent = sum(test_feats_2571 .* weight_i_parent, 2) + weight_0_parent;
% test_predictions_binary = sign(test_predictions_parent);
% 
% % 计算测试集准确率
% test_accuracy = sum(test_predictions_binary == (test_labels_2571 == 2 | test_labels_2571 == 5) * 2 - 1) / numel(test_labels_2571);
% fprintf('Test Accuracy: %.2f%%\n', test_accuracy * 100);
% ---------- test code end   ---------- %
% ------------------- parent perceptron train code end   ------------------- %

% ------------------- child perceptron train code begin ------------------- %
% 提取数字 '2' 和 '5' 的样本
indices_25 = find(labels == 2 | labels == 5);
feats_25 = feats(indices_25, :);
labels_25 = double(labels(indices_25));
% 提取数字 '1' 和 '7' 的样本
indices_17 = find(labels == 1 | labels == 7);
feats_17 = feats(indices_17, :);
labels_17 = double(labels(indices_17));

% 将 '2' 视为正类（+1），'5' 视为负类（-1）
labels_25_binary = (labels_25 == 2) * 2 - 1;
% 将 '1' 视为正类（+1），'7' 视为负类（-1）
labels_17_binary = (labels_17 == 1) * 2 - 1;

% 使用 Adaline 学习（子感知器1：区分 '2' 和 '5'）
[~, weight_i_child1, weight_0_child1] = adaline_train(feats_25, labels_25_binary, 0, 0.001, 100, 0.02);
% 使用 Adaline 学习（子感知器2：区分 '1' 和 '7'）
[~, weight_i_child2, weight_0_child2] = adaline_train(feats_17, labels_17_binary, 0, 0.001, 100, 0.02);

% ---------- test code begin ---------- %
% % 使用 Adaline 预测
% predictions_child1 = sum(feats_25 .* weight_i_child1, 2) + weight_0_child1;
% predictions_child2 = sum(feats_17 .* weight_i_child2, 2) + weight_0_child2;
% % 将 '2' 分类为 1，'5' 分类为 -1
% predictions_binary_child1 = sign(predictions_child1);
% predictions_binary_child2 = sign(predictions_child2);

% disp("size of child1 labels");
% disp(size(labels_25_binary));
% disp("size of child1 correct classification")
% disp(sum(predictions_binary_child1 == labels_25_binary))
% disp("size of child2 labels");
% disp(size(labels_parent))
% disp("size of child2 correct classification")
% disp(sum(predictions_binary_child2 == labels_17_binary))
% ---------- test code end   ---------- %

% ---------- test code begin ---------- %
% 计算子感知器1的准确率
% train_accuracy_child1 = sum(predictions_binary_child1 == labels_25_binary) / numel(labels_25_binary);
% fprintf('Training Accuracy of Child Perceiver 1: %.2f%%\n', train_accuracy_child1 * 100);
% % 计算子感知器2的准确率
% train_accuracy_child2 = sum(predictions_binary_child2 == labels_17_binary) / numel(labels_17_binary);
% fprintf('Training Accuracy of Child Perceiver 2: %.2f%%\n', train_accuracy_child2 * 100);
% ---------- test code end   ---------- %
% ------------------- child perceptron train code end   ------------------- %

% ------------------- Perceptron prediction code begin ------------------- %
% 预测感知器输出
predictions_parent = sum(test_feats_2571 .* weight_i_parent, 2) + weight_0_parent;
predictions_parent_binary = sign(predictions_parent);
predictions_child1 = sum(test_feats_2571 .* weight_i_child1, 2) + weight_0_child1;
predictions_child1_binary = sign(predictions_child1);
predictions_child2 = sum(test_feats_2571 .* weight_i_child2, 2) + weight_0_child2;
predictions_child2_binary = sign(predictions_child2);

% ---------- test code begin ---------- %
% disp("size of test labels");
% disp(size(test_feats_2571));
% disp("legth of predicted_labels");
% disp(length(test_labels_2571))
% ---------- test code end   ---------- %

% 找到对应数字的行索引
indice_2 = find(predictions_parent_binary == 1 & predictions_child1_binary == 1);
indice_5 = find(predictions_parent_binary == 1 & predictions_child1_binary ==-1);
indice_1 = find(predictions_parent_binary ==-1 & predictions_child2_binary == 1);
indice_7 = find(predictions_parent_binary ==-1 & predictions_child2_binary ==-1);
predicted_labels(indice_2) = 2;
predicted_labels(indice_5) = 5;
predicted_labels(indice_1) = 1;
predicted_labels(indice_7) = 7;

% 计算得到的是行向量，转化成列向量
predicted_labels = predicted_labels';

% 计算正确率
correct_predictions = (predicted_labels == test_labels_2571);
accuracy = sum(correct_predictions) / length(test_labels_2571) * 100;
fprintf('Accuracy: %.2f%%\n', accuracy);

% ---------- additional code begin ---------- %
% % 找到分类错误的索引
% misclassified_indices = find(predicted_labels ~= test_labels_2571);
% 
% % 输出每个错误分类的详细信息
% for i = 1:length(misclassified_indices)
%     idx = misclassified_indices(i);
%     fprintf('Sample %d: Predicted %d, Actual %d\n', idx, predicted_labels(idx), test_labels_2571(idx));
% end
% ---------- additional code end   ---------- %

% ------------------- Perceptron prediction code end   ------------------- %
