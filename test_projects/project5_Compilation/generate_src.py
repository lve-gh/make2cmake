
def generate_src_code():
    with open('src.cpp', 'w') as file:
        file.write('#include <iostream>\n\n')
        file.write('int main() {\n')
        file.write('\tint x = 50;\n')
        file.write('\tint y = 50;\n')
        file.write('\tint sum = x + y;\n')
        file.write('\tstd::cout << "Sum of {} and {} is: " << sum << std::endl;\n'.format('x', 'y'))
        file.write('\treturn 0;\n')
        file.write('}\n')

if __name__ == "__main__":
    generate_src_code()