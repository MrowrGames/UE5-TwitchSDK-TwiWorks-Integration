#include "marshalling.hpp"

using namespace R66;

void R66::Marshalling::ConsumeLambda(void* p, void* arg)
{
	auto payload = (std::function<void(void*)>*)p;
	(*payload)(arg);
	delete payload;
}

bool R66::Marshalling::StringArrayManagedIter::operator!=(const StringArrayManagedIter& other)
{
	return Index != other.Index;
}
R66::Marshalling::StringArrayManagedIter R66::Marshalling::StringArrayManagedIter::operator++()
{
	auto ret = *this;
	Index++;
	return ret;
}
std::u16string_view R66::Marshalling::StringArrayManagedIter::operator*()
{
	return std::u16string_view(Parent.Data[Index], Parent.Lengths[Index + 1]);
}
R66::Marshalling::StringArrayManaged::operator std::vector<R66::string_holder>() const
{
	std::vector<R66::string_holder> ret;
	ret.reserve(Length());
	for (auto s : *this) {
		ret.push_back(s);
	}
	return ret;
}


R66::Marshalling::StringArrayManaged::StringArrayManaged(const std::vector<R66::u16string_holder>& v)
	: Data(new const char16_t* [v.size()]), Lengths(new int32_t[v.size() + 1])
{
	auto num_eles = v.size();
	if (num_eles > INT32_MAX) throw std::range_error("string array too long");
	Lengths[0] = static_cast<int32_t>(num_eles);
	for (std::size_t i = 0; i < num_eles; i++) {
		auto& s = v[i];
		auto size = s.size();
		if (size > INT32_MAX) throw std::range_error("string in string array too long");
		Data[i] = s.data();
		Lengths[i + 1] = static_cast<int32_t>(size);
	}
}
R66::Marshalling::StringArrayManaged::~StringArrayManaged()
{
	delete[] Data;
	delete[] Lengths;
}
