data_load

for i = 1:10
    subplot(2, 5, i);
    img = reshape(train_images_array(i, :), [28, 28]);  % 将图像重新转换为 28x28 的形状
    imshow(img', 'InitialMagnification', 'fit');  % 转置图像矩阵并设置放大方式为适应窗口
    colormap(gray);  % 灰度图
    title(['Label: ', num2str(train_labels_array(i))]);
end
