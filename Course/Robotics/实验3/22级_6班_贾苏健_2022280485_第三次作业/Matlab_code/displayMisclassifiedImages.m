function displayMisclassifiedImages(images, labels, indices)
    % images: 包含所有图像的矩阵
    % indices: 被错误分类图像的索引

    figure;
    idx = 1;

    while true
        img_index = indices(idx);
        
        test_label = labels(img_index);
        
        if test_label == 1
            true_label = 2;
            predicted_label = 5;
        elseif test_label == -1
            true_label = 5;
            predicted_label = 2;
        end

        % 设置标题，显示真实标签和预测标签
        title_str = sprintf('Misclassified Image - Index: %d\nTrue Label: %d, Predicted Label: %d', img_index, true_label, predicted_label);
        img = reshape(images(img_index, :), [28, 28]);
        imshow(img', 'InitialMagnification', 'fit');
        title(title_str);

        % 等待用户按下键盘上的任意键
        pause;

        % 切换到下一张图像
        idx = idx + 1;

        % 如果所有图像都已经显示过，则退出循环
        if idx > numel(indices)
            break;
        end
    end
end
