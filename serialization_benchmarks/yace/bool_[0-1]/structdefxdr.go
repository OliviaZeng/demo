package goserbench

//go:generate -command genxdr go run ../../calmh/xdr/cmd/genxdr/main.go
//go:generate genxdr -o structdefxdr_generated.go structdefxdr.go
type XDRA struct {
	Name     bool 
	BirthDay bool
	Phone    bool
	Siblings bool
	Spouse   bool
	Money    bool
}
