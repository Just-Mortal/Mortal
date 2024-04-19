% 文件路径
train_images_file = 'train_data/train-images.idx3-ubyte';
train_labels_file = 'train_data/train-labels.idx1-ubyte';
test_images_file = 'test_data/t10k-images.idx3-ubyte';
test_labels_file = 'test_data/t10k-labels.idx1-ubyte';

% 加载训练集和测试集数据
train_images_array = load_mnist_images(train_images_file);
train_labels_array = load_mnist_labels(train_labels_file);
test_images_array = load_mnist_images(test_images_file);
test_labels_array = load_mnist_labels(test_labels_file);