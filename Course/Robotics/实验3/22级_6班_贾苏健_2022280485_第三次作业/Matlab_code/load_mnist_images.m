function data = load_mnist_images(filename)
    % 数据集图像加载函数
    fid = fopen(filename, 'rb');
    data = fread(fid, 'uint8');
    fclose(fid);
    data = data(17:end) / 255.0;
    data = reshape(data, 28*28, []).';
end