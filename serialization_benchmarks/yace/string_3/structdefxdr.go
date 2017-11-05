package goserbench

//go:generate -command genxdr go run ../../calmh/xdr/cmd/genxdr/main.go
//go:generate genxdr -o structdefxdr_generated.go structdefxdr.go
type XDRA struct {
	Name     string
	BirthDay string
	Phone    string
	Siblings string
	Spouse   string
	Money    string
}
