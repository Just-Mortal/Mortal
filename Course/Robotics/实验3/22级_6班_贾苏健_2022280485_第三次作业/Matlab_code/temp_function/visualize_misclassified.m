function visualize_misclassified(feats, labels, weight_i, weight_0, title_text)
    % 可视化被错误分类的样本

    % 计算感知器的预测值
    dot_product = feats * weight_i' + weight_0;
    predictions = sign(dot_product);

    % 提取被错误分类的索引
    misclassified_indices = find(predictions ~= labels);

    % 创建图像
    scatter(feats(misclassified_indices, 1), feats(misclassified_indices, 2), 'gx', 'DisplayName', 'Misclassified Samples');
    title(title_text);
    xlabel('Feature 1');
    ylabel('Feature 2');
    legend;
end
