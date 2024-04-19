function displayRandomDigitSample(images, labels, digit)
    % 找到属于特定数字的样本的索引
    digit_indices = find(labels == digit);

    % 显示数字的样本数量
    fprintf('Number of samples for digit %d: %d\n', digit, length(digit_indices));

    % 随机选择要显示的样本索引
    random_index = datasample(digit_indices, 1, 'Replace', false);

    % 获取图像数据
    image_data = reshape(images(random_index, :), [28, 28]);  % 假设图像大小是 28x28

    % 显示图像
    figure;
    imshow(image_data', 'InitialMagnification', 'fit');  % 转置图像矩阵并设置放大方式为适应窗口

    % 设置坐标轴
    axis off;

    % 显示标签
    title(['Label: ', num2str(labels(random_index))]);
end
