#include "../Shared/stdafx.h"
#include "assembler.h"

std::string infilename = "", outfilename = "";
std::ifstream inputfile;
std::ofstream outputfile;

opcodeManager manager;
class assembler* assembler = assembler::getInstance();

int main(int argc, char* argv[]) {
	opcodeManager::loadOpcodeList();

	for(size_t i = 0; i < argc; i++) {
		if(strcmp(argv[i], "-i") == 0 && (i + 1 < argc)) {
			infilename = argv[i + 1];
			PRINT(argv[i]);
		}
		if(strcmp(argv[i], "-o") == 0 && (i + 1 < argc)) {
			outfilename = argv[i + 1];
			PRINT(argv[i]);
		}
	}
	if(infilename == "") {
		infilename = "data.asm";
	}
	if(outfilename == "") {
		outfilename = infilename.substr(0, infilename.find_last_of(".")) + ".hex";
	}

	PRINT("Opening file " << infilename);
	inputfile.open(infilename, std::ifstream::in);


	std::string line;
	size_t nline = 0;
	while(getline(inputfile, line)) {
		std::vector<std::string> parsingLine{std::explode(line, ' ')};

		auto lineType = assembler::getLineType(line);
		switch(lineType) {
			case assembler::lineTypes::COMMENT:
				break;
			case assembler::lineTypes::DIRECTIVE:
				PRINT("Found processor directive: " << rang::bg::green << line << rang::style::reset);
				assembler->processDirective(parsingLine[0], parsingLine[1]);
				break;
			case assembler::lineTypes::LABEL:
				PRINT("" << rang::bg::magenta << "Found label " << line << "(" << HEX(assembler->nextAddress) << ")" << rang::style::reset);
				assembler->registerLabel(line, assembler->nextAddress);
				break;
			case assembler::lineTypes::OPCODE:
				if(parsingLine.size() == 1) parsingLine.push_back("");
				{
					PRINT("Compiling " << rang::fg::cyan << line << rang::style::reset);
					opcode readOpcode = opcodeManager::opcodeParser(parsingLine[0], parsingLine[1]);
					assembler->storeInROM(assembler->compileLine(readOpcode, parsingLine[1]));
				};
				break;
			default:
				break;
		}
		nline++;
	}

	int amountOfUndefinedLabels = 0;
	for(auto const&label : assembler->missingLabels) {
		PRINT(label.first << " couldn't be found");
		amountOfUndefinedLabels += label.second.size();
	}
	if(amountOfUndefinedLabels > 0) {
		PRINT(amountOfUndefinedLabels << " references to undefined lables");
		return 1;
	}

	PRINT("Finished compiling at " << rang::bg::green << HEX(assembler->nextAddress) << rang::style::reset);

	outputfile.open(getOutFileName(), std::ofstream::out);
	for(size_t addr = 0; addr < ROMSIZE; addr++) {
		outputfile << std::hex << assembler->ROM[addr];
	}

	return 0;
}