#include <nV/Serializer.h>
#include <stdexcept>

namespace nV {
Serializer serializer;
void var::save(ostream& o) const {
	serializer.save(*this, o);
}
var load(istream& i) {
	return serializer.load(i);
}
uint Serializer::save(const var& x, ostream& o) {
	Saving saving;
	saving.save(x, o);
	return saving.field_index + 1;
}
var Serializer::load(istream& i) {
	Loading loading;
	return loading.load(i);
}
uint Serializer::Saving::save(const var& x, ostream& o) {
	typedef std::unordered_map<var, uint>::iterator Iterator;
	std::pair<Iterator, bool>
    r = var_index.insert(std::make_pair(x, 0));	
    if (!r.second)
        return r.first->second;
	uint index;
	switch (x.primary()) {
	case Primary::Object:
		index = save(x.object(),o); r.first->second = index; return index;
    case Primary::Symbol:
        index = save(x.symbol(),o); r.first->second = index; return index;
    case Primary::Key:
        index = save(x.key(),o); r.first->second = index; return index;
    case Primary::Tuple:
        index = save(x.tuple(),o);  r.first->second = index; return index;
	default: throw std::logic_error("error in Serializer::save var");
    }
}
uint Serializer::Saving::save(sym x, ostream& o) {
	field_head head;
	head.primary = static_cast<wchar_t>(Primary::Symbol);
	head.index = (wchar_t)0;

	wostringstream ws; x->print(ws);
	head.value.size = ws.str().size();

	wstring s = ws.str();
	uint ssize = s.size();
	uint hsize = sizeof(field_head);

	if (!o.write((char*)&head, sizeof(field_head)) || !o.write((const char*)ws.str().c_str(), head.value.size*2)){
		throw std::logic_error("error in Serializer::save Symbol");
	}

	return field_index++;
}
uint Serializer::Saving::save(const Key& x,  ostream& o) {
	field_head head;
	head.primary = static_cast<wchar_t>(Primary::Key);

	switch (x.kind()) {
    case  Key::Null:
        head.index = (wchar_t)Key::Null;break;
    case Key::String:
		head.index = (wchar_t)Key::String; head.value.size = wcslen(x.toS());
		if (!o.write((const char*)&head, sizeof(field_head)) || !o.write((const char*)x.toS(), head.value.size*2)){
			throw std::logic_error("error in Serializer::save key String");
		}
		break;
    case Key::Integer:
        head.index = (wchar_t)Key::Integer; head.value.int_value = x.toUI(); 
		if (!o.write((const char*)&head, sizeof(field_head)) ){
			throw std::logic_error("error in Serializer::save key Integer");
		}
		break;
	default: throw std::logic_error("error in Serializer::save key");
    }

	return field_index++;
}
uint Serializer::Saving::save(const Tuple& x,  ostream& o) {
	field_head head;
	head.primary = static_cast<wchar_t>(Primary::Tuple);
	head.index = (wchar_t)0;
	head.value.size = x.size;
	uint* pu = new uint[x.size];
	
	for(uint i = 0; i < x.size; ++i) {
		pu[i] = save(x.tuple[i], o);
	}

	if (!o.write((const char*)&head, sizeof(field_head)) || !o.write((const char*)pu, x.size*sizeof(uint))){
		throw std::logic_error("error in Serializer::save tuple");
	}

	delete pu;
	return field_index++;
}
uint Serializer::Saving::save(const __mpz_struct* x, ostream& o){
	field_head head;
	head.primary = static_cast<wchar_t>(Primary::Object);
	head.index = (wchar_t)0;
	head.value.ssize = x->_mp_size;

	if (!o.write((const char*)&head, sizeof(field_head)) 
		|| !o.write((const char*)x->_mp_d, std::abs(head.value.ssize) * sizeof(mp_limb_t))){
		throw std::logic_error("error in Serializer::save Integer");
	}

	return field_index++;
}
uint Serializer::Saving::save(const __mpf_struct* x, ostream& o){
	field_head head;
	head.primary = static_cast<wchar_t>(Primary::Object);
	head.index = (wchar_t)2;
	head.value.ssize = x->_mp_size;

	if (!o.write((const char*)&head, sizeof(field_head)) || !o.write((const char*)&(x->_mp_exp), sizeof(mp_exp_t))
		|| !o.write((const char*)x->_mp_d, std::abs(head.value.ssize) * sizeof(mp_limb_t))){
		throw std::logic_error("error in Serializer::save Real");
	}

	return field_index++;
}

uint Serializer::Saving::save(const __mpq_struct* x, ostream& o){
	struct mpq_head{wchar_t primary;wchar_t index; uint num; uint den;} head;
	head.primary = static_cast<wchar_t>(Primary::Object);
	head.index = (wchar_t)1;
	head.num = save(&(x->_mp_num),o);
	head.den = save(&(x->_mp_den),o);
	if (!o.write((const char*)&head, sizeof(mpq_head))){
		throw std::logic_error("error in Serializer::save Rational");
	}

	return field_index++;
}
uint Serializer::Saving::save(const String& x, ostream& o){
	field_head head;
	head.primary = static_cast<wchar_t>(Primary::Object);
	head.index = (wchar_t)3;
	head.value.size = x.str.size();

	if (!o.write((const char*)&head, sizeof(field_head)) 
		|| !o.write((const char*)x.str.c_str(), head.value.size*2)){
		throw std::logic_error("error in Serializer::save String");
	}

	return field_index++;
}
uint Serializer::Saving::save(const Object& x, ostream& o){
	if (x.type == $.Integer){
		return save(x.cast<Integer>().mpz,o);
	}
	else if (x.type == $.Rational){
		return save(x.cast<Rational>().mpq,o);	
	}
	else if (x.type == $.Real){
		return save(x.cast<Real>().mpf,o);	
	}
	else if (x.type == $.String){
		return save(x.cast<String>(),o);
	}
	else throw std::logic_error("error in Serializer::save object");
}
Tuple* Serializer::Loading::load_tuple(istream& i, field_head& fh) {
	Tuple* r = tuple(fh.value.size);
	uint* pu = new uint[fh.value.size];
	if (!i.read((char*)pu, fh.value.size*sizeof(uint))){
		throw std::logic_error("error in Serializer::load_tuple");
	}
	for (uint k = 0; k < fh.value.size; ++k){
		if (pu[k] < index_var.size())
			r->tuple[k] = index_var[pu[k]];
		else
			throw std::logic_error("error in Serializer::load_tuple");
	}
	delete pu;
	return r;
}
Key* Serializer::Loading::load_key(istream& i, field_head& fh) {
	switch(fh.index){
		case (wchar_t)Key::Null: return 0;	//这里应该返回什么？
		case (wchar_t)Key::String: 
			{
				wstring str(fh.value.size+1, _W('\0'));
				if (!i.read((char*)&*str.begin(), fh.value.size*2)){
					throw std::logic_error("error in Serializer::load_key");
				}
				return key(str[0] == _W('#') ? wstr(str.c_str() + 1) : wstr(str.c_str()));
			}
		case (wchar_t)Key::Integer: return key(fh.value.int_value);
		default: throw std::logic_error("error in Serializer::load_key");
	}
}
namespace {
//需测试
void split_string(wstring &src, const wstring& s, std::vector<wstring>& result) {
	size_t begin = 0, index = 0;
	for (; (index = src.find_first_of(s, begin)) != wstring::npos; begin = index + 1){
		if (begin != index) result.push_back(src.substr(begin,index));
	}
	index = src.size();
	if (begin != index) result.push_back(src.substr(begin,index));
}
}
sym Serializer::Loading::load_symbol(istream& i, field_head &fh) {
	wstring str(fh.value.size+1, _W('\0'));
	std::vector<wstring> vs;

	if (!i.read((char*)&*str.begin(), fh.value.size*2)){
		throw std::logic_error("error in Serializer::load_symbol");
	}
	split_string(str,_W("`"), vs);

	sym c = root;
	std::vector<wstring>::const_iterator iter = vs.begin();
	for(; iter != vs.end(); ++iter){
		c = c->symbol(wstr((*iter).c_str()));
	}
	return c;
}
Object* Serializer::Loading::load_object(istream& i, field_head& fh) {
	Integer* integer; Rational *rational;
	Real* real; String *s;
	switch(fh.index){
	case (wchar_t)0: 
		{
			integer = new Integer();
			mpz_clear(integer->mpz);
			mpz_init2(integer->mpz, std::abs(fh.value.ssize) * sizeof(mp_limb_t) * 8);
			integer->mpz[0]._mp_size = fh.value.ssize;
			if (!i.read((char*)integer->mpz[0]._mp_d, std::abs(fh.value.ssize) * sizeof(mp_limb_t))){
				throw std::logic_error("error when load Integer");
			}
			return integer;
	case (wchar_t)1:
		{
			uint den;
			if (!i.read((char*)&den, sizeof(uint))){
				throw std::logic_error("error when load Rational");
			}
			if (fh.value.int_value >= index_var.size() || den >= index_var.size()){
				throw std::logic_error("error when load Rational");
			}
			rational = new Rational();
			mpq_set_num(rational->mpq, index_var[fh.value.int_value].cast<Integer>().mpz);
			mpq_set_den(rational->mpq, index_var[den].cast<Integer>().mpz);
			return rational;
		}
	case (wchar_t)2:
		{
			real = new Real(); 
			mpf_clear(real->mpf);
			mpf_init2(real->mpf, std::abs(fh.value.ssize) * sizeof(mp_limb_t) * 8);
			real->mpf[0]._mp_size = fh.value.ssize;
			if (!i.read((char*)&real->mpf[0]._mp_exp, sizeof(mp_exp_t)) || 
				!i.read((char*)real->mpf[0]._mp_d, std::abs(fh.value.ssize) * sizeof(mp_limb_t))){
				throw std::logic_error("error when load Real");
			}
			return real;
		}
	case (wchar_t)3:
		{
			s = new String(); s->str = wstring(fh.value.size+1, _W('\0'));
			if (!i.read((char*)&*(s->str.begin()), fh.value.size*2)){
				throw std::logic_error("error when load String");
			}
			return s;
		}
	case 4:throw std::logic_error("error when load object");
	}
	}
}
var Serializer::Loading::load(istream& i) {
	field_head head;
	while (i.read((char*)&head, sizeof(field_head))){
		switch (head.primary) {
		case static_cast<wchar_t>(Primary::Tuple):index_var.push_back(load_tuple(i, head));break;
		case static_cast<wchar_t>(Primary::Key):index_var.push_back(load_key(i, head));break;
		case static_cast<wchar_t>(Primary::Object):index_var.push_back(load_object(i, head));break;
		case static_cast<wchar_t>(Primary::Symbol):index_var.push_back(load_symbol(i, head));break;
		default: throw std::logic_error("error in load");
		}
	}
	var r = index_var.back();
	index_var.clear();
	return r;
}
}