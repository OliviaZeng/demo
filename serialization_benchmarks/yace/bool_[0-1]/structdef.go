package goserbench

//go:generate msgp -o msgp_gen.go -io=false -tests=false
//easyjson:json
type A struct {
	Name     bool
	BirthDay bool
	Phone    bool
	Siblings bool
	Spouse   bool
	Money    bool
}
