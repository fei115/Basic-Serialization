# Basic-Serialization
Basic C++ serialization

Supports serialization and deserialization of the following types:
- Plain old data types: char, short, int, long, double, float, uint16_t, uint32_t
- Complex object types: string, Serializable, NoSwap(bypass endianness)
- Standard template library: array, vector, list

Also handles the following:
- endianness: automatically convert bytes to use big endian ordering during serialization(object to bytes). Automatically convert bytes to respect host endianness during deserialization(bytes to object).
- automatic endianness can be disabled by wrapping the object with NoSwap<T>
