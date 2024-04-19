function visualize_samples(feats, labels, weight_i, weight_0, title_text)
    % 计算感知器的预测值
    % 计算感知器的预测值
    dot_product = feats * weight_i' + weight_0;
    predictions = sign(dot_product);

    % 提取预测为正类和负类的索引
    positive_indices = find(predictions == 1);
    negative_indices = find(predictions == -1);

    % 创建图像
    scatter(feats(positive_indices, 1), feats(positive_indices, 2), 'bo', 'DisplayName', 'Predicted 2');
    hold on;
    scatter(feats(negative_indices, 1), feats(negative_indices, 2), 'rx', 'DisplayName', 'Predicted 5');
    
    % 使用标签信息区分颜色
    scatter(feats(labels == 1, 1), feats(labels == 1, 2), 'go', 'DisplayName', 'Actual 2');
    scatter(feats(labels == -1, 1), feats(labels == -1, 2), 'mo', 'DisplayName', 'Actual 5');
    
    title(title_text);
    xlabel('Feature 1');
    ylabel('Feature 2');
    legend;
    hold off;
end
