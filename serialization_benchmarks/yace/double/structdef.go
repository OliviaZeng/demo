package goserbench

//go:generate msgp -o msgp_gen.go -io=false -tests=false
//easyjson:json
type A struct {
	Name     float64
	BirthDay float64
	Phone    float64
	Siblings float64
	Spouse   float64
	Money    float64
}
