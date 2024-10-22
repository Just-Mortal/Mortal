% 运行之前需要先载入数据集
data_load

% 直观感受 23456 比较相似，01789 比较相似
% ------------------- Layer1 perceptron train code begin ------------------- %
% 转化成 double 型 和 weight 计算
train_images = double(train_images_array);  % 60000 * 784
train_labels = double(train_labels_array);  % 60000 * 1
test_images   = double(test_images_array);   % 10000 * 784
test_labels  = double(test_labels_array);   % 10000 * 1

% 将 '2.3.4.5.6' 视为正类（+1），'0.1.7.8.9' 视为负类（-1）
labels_layer1 = (train_labels == 2 | ...
                 train_labels == 3 | ...
                 train_labels == 4 | ...
                 train_labels == 5 | ...
                 train_labels == 6 ) * 2 - 1;

% 使用 Adaline 学习（Layer1 感知器：区分 '2.3.4.5.6' 和 '0.1.7.8.9'）
[~, weight_i_layer1_preceptron, weight_0_layer1_preceptron] = adaline_train(train_images, labels_layer1, 0, 0.001, 100, 0.02);
% ------------------- Layer1 perceptron train code end   ------------------- %

% ------------------- Layer2 perceptron train code begin ------------------- %
% ---------- 区分数字 '2.3' 和 '4.5.6' 的样本 code begin ---------- %
indices_23456 = find(train_labels == 2 | train_labels == 3 | train_labels == 4 | train_labels == 5 | train_labels == 6);
images_23456 = train_images(indices_23456, :);
labels_23456 = train_labels(indices_23456);

% 将 '2.3' 视为正类（+1），'4.5.6' 视为负类（-1）
binary_23456 = (labels_23456 == 2 | labels_23456 == 3) * 2 - 1;

% 使用 Adaline 学习（Layer2 感知器1：区分 '2.3' 和 '4.5.6'）
[~, weight_i_layer2_preceptron1, weight_0_layer2_preceptron1] = ...
    adaline_train(images_23456, binary_23456, 0, 0.001, 100, 0.02);
% ---------- 区分数字 '2.3' 和 '4.5.6' 的样本 code end   ---------- %

% ---------- 区分数字 '0.8' 和 '1.7.9' 的样本 code begin ---------- %
indices_01789 = find(train_labels == 0 | train_labels == 1 | train_labels == 7 | train_labels == 8 | train_labels == 9);
images_01789 = train_images(indices_01789, :);
labels_01789 = train_labels(indices_01789);

% 将 '0.8' 视为正类（+1），'1.7.9' 视为负类（-1）
binary_01789 = (labels_01789 == 0 | labels_01789 == 8) * 2 - 1;

% 使用 Adaline 学习（Layer2 感知器2：区分 '0.8' 和 '1.7.9'）
[~, weight_i_layer2_preceptron2, weight_0_layer2_preceptron2] = ...
    adaline_train(images_01789, binary_01789, 0, 0.001, 100, 0.02);
% ---------- 区分数字 '0.8' 和 '1.7.9' 的样本 code end   ---------- %
% ------------------- Layer2 perceptron train code end ------------------- %

% ------------------- Layer3 perceptron train code begin ------------------- %
% ---------- 区分数字 '2' 和 '3' 的样本 code begin ---------- %
indices_23 = find(train_labels == 2 | train_labels == 3);
images_23 = train_images(indices_23, :);
labels_23 = train_labels(indices_23);

% 将 '2' 视为正类（+1），'3' 视为负类（-1）
binary_23 = (labels_23 == 2) * 2 - 1;

% 使用 Adaline 学习（Layer3 感知器1：区分 '2' 和 '3'）
[~, weight_i_layer3_preceptron1, weight_0_layer3_preceptron1] = ...
    adaline_train(images_23, binary_23, 0, 0.001, 100, 0.02);
% ---------- 区分数字 '2' 和 '3' 的样本 code end   ---------- %

% ---------- 区分数字 '5.6' 和 '4' 的样本 code begin ---------- %
indices_456 = find(train_labels == 4 | train_labels == 5 | train_labels == 6);
images_456 = train_images(indices_456, :);
labels_456 = train_labels(indices_456);

% 将 '5.6' 视为正类（+1），'4' 视为负类（-1）
binary_456 = (labels_456 == 5 | labels_456 == 6) * 2 - 1;

% 使用 Adaline 学习（Layer3 感知器2：区分 '5.6' 和 '4'）
[~, weight_i_layer3_preceptron2, weight_0_layer3_preceptron2] = ...
    adaline_train(images_456, binary_456, 0, 0.001, 100, 0.02);
% ---------- 区分数字 '5.6' 和 '4' 的样本 code end   ---------- %

% ---------- 区分数字 '0' 和 '8' 的样本 code begin ---------- %
indices_08 = find(train_labels == 0 | train_labels == 8);
images_08 = train_images(indices_08, :);
labels_08 = train_labels(indices_08);

% 将 '0' 视为正类（+1），'8' 视为负类（-1）
binary_08 = (labels_08 == 0) * 2 - 1;

% 使用 Adaline 学习（Layer3 感知器3：区分 '0' 和 '8'）
[~, weight_i_layer3_preceptron3, weight_0_layer3_preceptron3] = ...
    adaline_train(images_08, binary_08, 0, 0.001, 100, 0.02);
% ---------- 区分数字 '0' 和 '8' 的样本 code end   ---------- %

% ---------- 区分数字 '1.7' 和 '9' 的样本 code begin ---------- %
indices_179 = find(train_labels == 1 | train_labels == 7 | train_labels == 9);
images_179 = train_images(indices_179, :);
labels_179 = train_labels(indices_179);

% 将 '1.7' 视为正类（+1），'9' 视为负类（-1）
binary_179 = (labels_179 == 1 | labels_179 == 7) * 2 - 1;

% 使用 Adaline 学习（Layer3 感知器4：区分 '1.7' 和 '9'）
[~, weight_i_layer3_preceptron4, weight_0_layer3_preceptron4] = ...
    adaline_train(images_179, binary_179, 0, 0.001, 100, 0.02);
% ---------- 区分数字 '1.7' 和 '9' 的样本 code end   ---------- %
% ------------------- Layer3 perceptron train code end ------------------- %

% ------------------- Layer4 perceptron train code begin ------------------- %
% ---------- 区分数字 '5' 和 '6' 的样本 code begin ---------- %
indices_56 = find(train_labels == 5 | train_labels == 6);
images_56 = train_images(indices_56, :);
labels_56 = train_labels(indices_56);

% 将 '5' 视为正类（+1），'6' 视为负类（-1）
binary_56 = (labels_56 == 5 ) * 2 - 1;

% 使用 Adaline 学习（Layer4 感知器1：区分 '5' 和 '6'）
[~, weight_i_layer4_preceptron1, weight_0_layer4_preceptron1] = ...
    adaline_train(images_56, binary_56, 0, 0.001, 100, 0.02);
% ---------- 区分数字 '5' 和 '6' 的样本 code end   ---------- %

% ---------- 区分数字 '1' 和 '7' 的样本 code begin ---------- %
indices_17 = find(train_labels == 1 | train_labels == 7);
images_17 = train_images(indices_17, :);
labels_17 = train_labels(indices_17);

% 将 '1' 视为正类（+1），'7' 视为负类（-1）
binary_17 = (labels_17 == 1) * 2 - 1;

% 使用 Adaline 学习（Layer4 感知器2：区分 '1' 和 '7'）
[~, weight_i_layer4_preceptron2, weight_0_layer4_preceptron2] = ...
    adaline_train(images_17, binary_17, 0, 0.001, 100, 0.02);
% ---------- 区分数字 '1' 和 '7' 的样本 code end   ---------- %
% ------------------- Layer4 perceptron train code end ------------------- %

% ------------------- Perceptron prediction code begin ------------------- %
% 预测感知器输出
predictions_layer1_P = sum(test_images .* weight_i_layer1_preceptron, 2) + weight_0_layer1_preceptron;
predictions_layer1_P_binary = sign(predictions_layer1_P);

predictions_layer2_P1 = sum(test_images .* weight_i_layer2_preceptron1, 2) + weight_0_layer2_preceptron1;
predictions_layer2_P1_binary = sign(predictions_layer2_P1);
predictions_layer2_P2 = sum(test_images .* weight_i_layer2_preceptron2, 2) + weight_0_layer2_preceptron2;
predictions_layer2_P2_binary = sign(predictions_layer2_P2);

predictions_layer3_P1 = sum(test_images .* weight_i_layer3_preceptron1, 2) + weight_0_layer3_preceptron1;
predictions_layer3_P1_binary = sign(predictions_layer3_P1);
predictions_layer3_P2 = sum(test_images .* weight_i_layer3_preceptron2, 2) + weight_0_layer3_preceptron2;
predictions_layer3_P2_binary = sign(predictions_layer3_P2);
predictions_layer3_P3 = sum(test_images .* weight_i_layer3_preceptron3, 2) + weight_0_layer3_preceptron3;
predictions_layer3_P3_binary = sign(predictions_layer3_P3);
predictions_layer3_P4 = sum(test_images .* weight_i_layer3_preceptron4, 2) + weight_0_layer3_preceptron4;
predictions_layer3_P4_binary = sign(predictions_layer3_P4);

predictions_layer4_P1 = sum(test_images .* weight_i_layer4_preceptron1, 2) + weight_0_layer4_preceptron1;
predictions_layer4_P1_binary = sign(predictions_layer4_P1);
predictions_layer4_P2 = sum(test_images .* weight_i_layer4_preceptron2, 2) + weight_0_layer4_preceptron2;
predictions_layer4_P2_binary = sign(predictions_layer4_P2);

% 找到对应数字的行索引
indice_2 = find(predictions_layer1_P_binary == 1 & predictions_layer2_P1_binary == 1 & predictions_layer3_P1_binary == 1);
indice_3 = find(predictions_layer1_P_binary == 1 & predictions_layer2_P1_binary == 1 & predictions_layer3_P1_binary ==-1);
indice_5 = find(predictions_layer1_P_binary == 1 & predictions_layer2_P1_binary ==-1 & predictions_layer3_P2_binary == 1 & predictions_layer4_P1_binary == 1);
indice_6 = find(predictions_layer1_P_binary == 1 & predictions_layer2_P1_binary ==-1 & predictions_layer3_P2_binary == 1 & predictions_layer4_P1_binary ==-1);
indice_4 = find(predictions_layer1_P_binary == 1 & predictions_layer2_P1_binary ==-1 & predictions_layer3_P2_binary ==-1);
indice_0 = find(predictions_layer1_P_binary ==-1 & predictions_layer2_P2_binary == 1 & predictions_layer3_P3_binary == 1);
indice_8 = find(predictions_layer1_P_binary ==-1 & predictions_layer2_P2_binary == 1 & predictions_layer3_P3_binary ==-1);
indice_1 = find(predictions_layer1_P_binary ==-1 & predictions_layer2_P2_binary ==-1 & predictions_layer3_P4_binary == 1 & predictions_layer4_P2_binary == 1);
indice_7 = find(predictions_layer1_P_binary ==-1 & predictions_layer2_P2_binary ==-1 & predictions_layer3_P4_binary == 1 & predictions_layer4_P2_binary ==-1);
indice_9 = find(predictions_layer1_P_binary ==-1 & predictions_layer2_P2_binary ==-1 & predictions_layer3_P4_binary ==-1);

predicted_labels(indice_0) = 0;
predicted_labels(indice_1) = 1;
predicted_labels(indice_2) = 2;
predicted_labels(indice_3) = 3;
predicted_labels(indice_4) = 4;
predicted_labels(indice_5) = 5;
predicted_labels(indice_6) = 6;
predicted_labels(indice_7) = 7;
predicted_labels(indice_8) = 8;
predicted_labels(indice_9) = 9;

% 计算得到的是行向量，转化成列向量
predicted_labels = predicted_labels';

% 计算正确率
correct_predictions = (predicted_labels == test_labels);
accuracy = sum(correct_predictions) / length(test_labels) * 100;
fprintf('Accuracy: %.2f%%\n', accuracy);
% ------------------- Perceptron prediction code end ------------------- %

% ------------------- Display misclassified images code begin ------------------- %
% Initialize variables for misclassified images
misclassified_indices = find(correct_predictions == 0);
num_misclassified = length(misclassified_indices);

% Display up to ten misclassified images
for i = 1:min(num_misclassified, 10)
    % Get the index of the current misclassified image
    current_image_index = misclassified_indices(i);
    
    % Display the misclassified image
    img = reshape(test_images(current_image_index, :), [28, 28]);
    subplot(2, 5, i);
    imshow(img', 'InitialMagnification', 'fit');
    colormap(gray);  % 灰度图
    title(['True: ', num2str(test_labels(current_image_index)), ', Predicted: ', num2str(predicted_labels(current_image_index))]);
end
% ------------------- Display misclassified images code end   ------------------- %