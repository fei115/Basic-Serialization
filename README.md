# Basic-Serialization
Basic C++ serialization

Supports serialization and deserialization of the following types:
- Plain old data types: char, short, int, long, double, float, uint16_t, uint32_t
- Complex object types: string, Serializable, NoSwap(bypass endianness)
- Standard template library: array, vector, list

Also handles the following:
- endianness: automatically converted to big endian during serialization. can be disabled using NoSwap<T>
