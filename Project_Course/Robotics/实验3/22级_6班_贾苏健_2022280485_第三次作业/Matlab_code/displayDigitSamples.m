function displayDigitSamples(images, labels, digit)
    % 找到属于特定数字的样本的索引
    digit_indices = find(labels == digit);

    % 显示数字的样本数量
    fprintf('Number of samples for digit %d: %d\n', digit, length(digit_indices));

    % 询问用户要显示的样本的序号
    prompt = 'Enter the sample index you want to display: ';
    sample_index = input(prompt);

    % 显示所选数字的样本图像
    if sample_index >= 1 && sample_index <= length(digit_indices)
        % 获取图像数据
        image_data = reshape(images(digit_indices(sample_index), :), [28, 28]);  % 假设图像大小是 28x28

        % 显示图像
        figure;
        imshow(image_data', 'InitialMagnification', 'fit');  % 转置图像矩阵并设置放大方式为适应窗口
        colormap(gray);  % 灰度图
        title(['Label: ', num2str(labels(digit_indices(sample_index)))]);
    else
        fprintf('Invalid sample index.\n');
    end
end
