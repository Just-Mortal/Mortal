function [weight_i, weight_0] = initialize_weights(input_variables_size, weights_init_val)
% 初始化权重
    if nargin < 2 || isempty(weights_init_val)
        weight_i = rand(1, input_variables_size);
        weight_0 = rand(1);
    else
        weight_i = ones(1, input_variables_size) * weights_init_val;
        weight_0 = ones(1) * weights_init_val;
    end
end