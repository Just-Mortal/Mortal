function num_of_sample = calculateSampleCount(labels)
    num_of_sample = zeros(1, 10);

    for i = 0:9
        num_of_sample(i + 1) = sum(labels == i);
    end

    disp('Number of samples for each digit:');
    disp(num_of_sample);
end
