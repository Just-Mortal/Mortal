% 运行之前需要先载入数据集
data_load

% 提取数字 '2' 和 '5' 的样本
indices_2_5 = find(train_labels_array == 2 | train_labels_array == 5);

feats = double(train_images_array(indices_2_5, :));
labels = double(train_labels_array(indices_2_5));

% 将 '2' 视为正类（+1），'5' 视为负类（-1）
labels = (labels == 2) * 2 - 1;

% 使用 Adaline 学习
[~, weight_i, weight_0] = adaline_train(feats, labels, 0, 0.001, 100, 0.02);

% 筛选标签为2和5的样本
test_feats_2_5 = double(test_images_array(test_labels_array == 2 | test_labels_array == 5, :));
test_labels_2_5 = double(test_labels_array(test_labels_array == 2 | test_labels_array == 5));

% 将 '2' 视为正类（+1），'5' 视为负类（-1）
test_labels_2_5 = (test_labels_2_5 == 2) * 2 - 1;

% 使用 Adaline 预测
predictions_2_5 = sum(test_feats_2_5 .* weight_i, 2) + weight_0;

% 找到最接近 '2' 和 '5' 的样本
closest_2_index = find(abs(predictions_2_5 - 1) == min(abs(predictions_2_5 - 1)) & test_labels_2_5 == 1, 1);
closest_5_index = find(abs(predictions_2_5 + 1) == min(abs(predictions_2_5 + 1)) & test_labels_2_5 ==-1, 1);

% 找到被错误分类的样本
misclassified_indices = find(sign(predictions_2_5) ~= test_labels_2_5);

% --- test code begin --- %

% disp('Unique labels in test_labels_2_5:');
% disp(unique(test_labels_2_5));
% 
% disp(size(predictions_2_5));
% disp('closest_2_index' );
% disp(closest_2_index);
% disp('closest_5_index');
% disp(closest_5_index);
% disp(size(test_feats_2_5(closest_2_index, : )));

% --- test code end   --- %

% 可视化结果
figure;

% 最接近 '2' 的样本
subplot(1, 2, 1);
img = reshape(test_feats_2_5(closest_2_index, :), [28, 28]);
imshow(img', 'InitialMagnification', 'fit');
title('Closest to 2');


% 最接近 '5' 的样本
subplot(1, 2, 2);
img = reshape(test_feats_2_5(closest_5_index, :), [28, 28]);
imshow(img', 'InitialMagnification', 'fit');
title('Closest to 5');

% --- test code begin --- % 

% figure;
% img = reshape(test_feats_2_5(misclassified_indices(1), :), [28, 28]);
% imshow(img', 'InitialMagnification', 'fit');
% string = sprintf('Misclassified Image.True Label:%d', test_labels_2_5(misclassified_indices(1)));
% title(string);

% --- test code end   --- % 

% 被错误分类的样本
fprintf('Number of misclassified samples: %d\n', length(misclassified_indices));
displayMisclassifiedImages(test_feats_2_5, test_labels_2_5, misclassified_indices)

% --- test code begin --- % 

% disp(size(test_feats_2_5))
% disp(size(weight_i))
% disp(size(weight_0))
% disp(size(predictions_2_5));
% disp(size(test_labels_2_5));

% --- test code end   --- % 
