package goserbench

//go:generate -command genxdr go run ../../calmh/xdr/cmd/genxdr/main.go
//go:generate genxdr -o structdefxdr_generated.go structdefxdr.go
type XDRA struct {
	Name     uint64
	BirthDay uint64
	Phone    uint64
	Siblings uint64
	Spouse   uint64
	Money    uint64
}
