function [errors, final_weight_i, final_weight_0] = single_perceptron_train(x, y, weights_init_val, learning_rate, epochs, stopping_threshold)
% 训练函数
    input_variables_size = size(x, 2);
    [weight_i, weight_0] = initialize_weights(input_variables_size, weights_init_val);

    errors = zeros(1, epochs);

    for epoch = 1:epochs
        error = 0;
        for sample = 1:size(x, 1)
            dot_product = sum(x(sample, :) .* weight_i);
            prediction = sign(dot_product + weight_0);
            update = learning_rate * (y(sample) - prediction);
            weight_i = weight_i + update * x(sample, :);
            weight_0 = weight_0 + update;
            error = error + (update ~= 0);
        end

        errors(epoch) = error / size(y, 1);

        if errors(epoch) < stopping_threshold
            break;
        end
    end

    final_weight_i = weight_i;
    final_weight_0 = weight_0;
end