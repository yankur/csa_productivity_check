import subprocess
import random
import sys


def run(n_times):
    print('Counting first word, microseconds:')
    for i in range(1, 4):
        output = open(file_out, "a+")
        output.write('Method {}: \n'.format(i))
        output.close()
        min_time = 10000000
        not_correct = 0
        for j in range(n_times):
            output = open(file_out, "a+")
            output.write('#{}: result: '.format(j+1))
            output.close()
            python_result = generate_file()
            out = subprocess.Popen(['./test', str(i), file_in, file_out],
                                   stdout=subprocess.PIPE,
                                   stderr=subprocess.STDOUT)
            stdout, stderr = out.communicate()
            stdout = stdout.split()
            min_time = min(min_time, int(stdout[-1]))
            output = open(file_out, 'ra+')
            result = output.readlines()[-1].split()[-1]

            correct = True
            if python_result != int(result):
                not_correct += 1
                correct = False

            output.write(', correct: {}'.format(correct))
            output.write(', time: {} \n'.format(stdout[-1]))
            output.close()
            if python_result != int(result):
                not_correct += 1

        output = open(file_out, "a+")
        output.write('\n')
        output.close()
        print('Method {}: time {}, correctness {}%'.format(i, min_time, ((n_of_times-not_correct)/n_of_times)*100))


def generate_file():
    with open('words.txt') as f:
        word_list = [word.replace('\n', ' ') for word in f.readlines()]

    input_file = open(file_in, 'w+')
    for i in range(200000):
        input_file.write(random.choice(word_list))
    input_file.close()

    input_file = open(file_in, 'r')
    words = input_file.readlines()
    words = words[0].split(' ')
    python_count = words.count(words[0])
    input_file.close()

    return python_count


n_of_times = int(sys.argv[1])
file_in = sys.argv[2]
file_out = sys.argv[3]
input_f = open(file_out, 'w+')
input_f.write('')
input_f.close()
run(n_of_times)
