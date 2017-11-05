package goserbench

//go:generate -command genxdr go run ../../calmh/xdr/cmd/genxdr/main.go
//go:generate genxdr -o structdefxdr_generated.go structdefxdr.go
type XDRA struct {
	Name     int32
	BirthDay int32
	Phone    int32
	Siblings int32
	Spouse   int32
	Money    int32
}
