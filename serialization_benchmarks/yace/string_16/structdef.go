package goserbench

//go:generate msgp -o msgp_gen.go -io=false -tests=false
//easyjson:json
type A struct {
	Name     string
	BirthDay string
	Phone    string
	Siblings string
	Spouse   string
	Money    string
}
