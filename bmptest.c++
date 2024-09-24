#include <deque>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <vector>

size_t read4(std::istream &F)
{
	size_t ret = 0, factor = 1;
	for (int i = 0; i < 4; ++i) {
		char c;
		if (!F.read(&c, 1))
			throw std::runtime_error("End of file");
		ret += (unsigned char)c * factor;
		factor *= 256;
	}
	return ret;
}

void render_file(std::istream &F)
{
	char S1, S2;
	if (!(F >> S1 >> S2) || S1 != 'B' || S2 != 'M')
		throw std::runtime_error("Wrong file signature");
	size_t size = read4(F);
	read4(F);
	size_t offset = read4(F);
	size_t dib_size = read4(F);
	size_t width = read4(F);
	size_t height = read4(F);
	//std::cerr << size << " " << offset << " " << dib_size << " " << width << " " << height << std::endl;
	F.seekg(offset, std::ios::beg);
	char pix[3];
	std::deque<std::vector<bool>> pixels;
	for (size_t row = 0; row < height; ++row) {
		pixels.push_front(std::vector<bool>());
		for (size_t column = 0; column < width; ++column) {
			F.read(pix, sizeof(pix));
			pixels.front().push_back(pix[0] || pix[1] || pix[2]);
		}
	}
	
	//output
	for (const auto &row : pixels) {
		for (bool p : row) {
			std::cout << (p ? "X" : " ");
		}
		std::cout << std::endl;
	}
}

int main(int argc, char *argv[])
{
	if (argc < 2) {
		std::cerr << "Usage: " << argv[0] << " file.bmp" << std::endl;
		return 255;
	}

	try {
		std::ifstream file(argv[1], std::ios::in | std::ios::binary);
		render_file(file);
	} catch (const std::exception &exc) {
		std::cerr << "Exception: " << exc.what() << std::endl;
		return 1;
	}
}
