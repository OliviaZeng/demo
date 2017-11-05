using Go = import "/github.com/glycerine/go-capnproto/go.capnp";
$Go.package("goserbench");
$Go.import("github.com/alecthomas/go_serialization_benchmarks");

@0x99ea7c74456111bd;

struct CapnpA {
  name     @0   :Float64;
  birthDay @1   :Float64;
  phone    @2   :Float64;
  siblings @3   :Float64;
  spouse   @4   :Float64;
  money    @5   :Float64;
}
