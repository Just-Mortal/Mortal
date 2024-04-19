function [train_errors, final_weight_i, final_weight_0] = adaline_train(x, y, weights_init_val,learning_rate, epochs, stopping_threshold)
% ADALINE 训练
    input_variables_size = size(x, 2);
    [weight_i, weight_0] = initialize_weights(input_variables_size, weights_init_val);

    train_errors = zeros(1, epochs);  % 预分配内存

    for epoch = 1:epochs
        train_error = 0;
        for i = 1:size(x, 1)
            xi = x(i, :);
            target = y(i);
            dot_product = sum(xi .* weight_i);
            prediction = dot_product + weight_0;
            update = learning_rate * (target - prediction);
            weight_i = weight_i + update * xi;
            weight_0 = weight_0 + update;
            train_error = train_error + (target - prediction)^2;  % 使用平方损失函数
        end

        train_mean_squared_error = train_error / size(y, 1);
        train_errors(epoch) = train_mean_squared_error;

        if train_mean_squared_error < stopping_threshold
%             train_errors = train_errors[:epoch + 1]  
%             % 截取数组长度，以适应变化的 stopping_threshold
            break;
        end
    end

    % 返回最终权重
    final_weight_i = weight_i;
    final_weight_0 = weight_0;
end