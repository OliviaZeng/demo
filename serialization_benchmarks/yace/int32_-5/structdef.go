package goserbench

//go:generate msgp -o msgp_gen.go -io=false -tests=false
//easyjson:json
type A struct {
	Name     int
	BirthDay int
	Phone    int
	Siblings int
	Spouse   int
	Money    int
}
