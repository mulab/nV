#include <nV/Kernel.h>
#include <nV/Parser.h>
#include <nV/utils.h>
#include <stdexcept>
#include <vector>

namespace nV {
uint Parser::load(const char* file_name) {
	if (!loading)
		loading = new Loading;
	loading->start(file_name, mNode, mNote);
	loading->load();
	return loading->rootindex();
}
void Parser::save(const char* s) {
	std::vector<uint> rootIndexs;
    while (parsing->mLookahead != EOI) {
        try {
			rootIndexs.push_back(parse());
        } catch (std::exception&) {
            kernel.logging(__FUNCTIONW__) << _W("Parse error at line ") << parsing->lineno
                                          << _W(" column ") << parsing->column << _W(".") << endl;
            throw;
        }
    }
	save(s, rootIndexs);
}
void Parser::save(const char* file_name, std::vector<uint>& rootIndex){
	std::string path_prefix = dump_path() + std::string(file_name);

	std::string node_path = path_prefix + std::string("_node");
	std::string note_path = path_prefix + std::string("_note");
	std::string rootindex_path = path_prefix + std::string("_rootindex.txt");

	std::ofstream of_node(node_path.c_str(), ios::binary | ios::trunc);
	std::ofstream of_note(note_path.c_str(), ios::binary | ios::trunc);
	std::ofstream of_rootindex(rootindex_path.c_str(), ios::trunc);

	if (!of_note || !of_note || !of_rootindex){
		throw std::logic_error("the file can not be opende in function text1");
	}

//	of_node.write((char*)(&(*(mNode.begin()))), sizeof(Node)*mNode.size());

	std::vector<Node>::const_iterator node_iter = mNode.begin();
	for (; node_iter != mNode.end(); ++node_iter){
		if (!of_node.write((char*)(&*node_iter), sizeof(NodeStore))){
			throw std::logic_error("write node error in function text1");
		}
	}

	
	std::vector<wstring>::const_iterator iter_note = mNote.begin();
	for (; iter_note != mNote.end(); ++iter_note) {
		//of_note << *iter_note << ' ';
		uint size = (*iter_note).size()*2;
		of_note.write((const char*)(&size), sizeof(uint));
		of_note.write((const char*)(*iter_note).c_str(), size);
	}

	std::vector<uint>::const_iterator iter_rootindex = rootIndex.begin();
	for (; iter_rootindex != rootIndex.end(); ++iter_rootindex){
		of_rootindex << *iter_rootindex << ' ';
	}

	of_node.close();
	of_note.close();
}
void Parser::Loading::load(){
	std::string path_prefix = dump_path() + std::string(file_name);
	std::string node_path = path_prefix + std::string("_node");
	std::string note_path = path_prefix + std::string("_note");
	std::string rootindex_path = path_prefix + std::string("_rootindex.txt");

	ifstream if_node(node_path.c_str(), ios::binary);
	ifstream if_note(note_path.c_str(), ios::binary);
	ifstream if_rootindex(rootindex_path.c_str());


	if (!if_node || !if_note || !if_rootindex){
		throw std::logic_error("the file can not be opende in function load1");
	}
	load(if_node, if_note, if_rootindex);
	if_node.close();
	if_note.close();
	if_rootindex.close();
}
void Parser::Loading::load(std::ifstream& i_node, std::ifstream& i_note, std::ifstream& i_rootindex) {
	Node tmp; uint index = 0;
	i_node.read((char*)&tmp, sizeof(NodeStore));
	while(i_node) {
		tmp.treenode.index = index++;
		pNode->push_back(tmp);
		i_node.read((char*)&tmp, sizeof(NodeStore));
	}
	
	uint size;
	wstring ws;
	/*while(std::getline(i_note, ws, _W(' '))) {
		pNote->push_back(ws);
	}*/
	uint count = sizeof(uint)/sizeof(wchar_t);
	while(i_note.read((char*)&size, sizeof(uint))){
		wstring pws(size/2 + 1,0);
		if (!i_note.read((char*)&*(pws.begin()), size)){
			i_note.close();
			i_rootindex.close();		//如果不注释掉此句，在release版本会发生错误，很诡异的错误，不知道为啥？
			throw std::logic_error("the note file is wrong!");
		}
		pNote->push_back(pws);
	}

	uint i;
	while(i_rootindex >> i){
		mRootIndex.push_back(i);
	}
}
}
