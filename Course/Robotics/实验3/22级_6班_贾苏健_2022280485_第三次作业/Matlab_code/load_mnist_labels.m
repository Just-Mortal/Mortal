function data = load_mnist_labels(filename)
% 数据集标签加载函数
    fid = fopen(filename, 'rb');
    data = fread(fid, 'uint8');
    fclose(fid);
    data = data(9:end);
end