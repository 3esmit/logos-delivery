fn main() {
    println!("cargo:rustc-link-arg-bin=waku-rust-simple-example=-llogosdelivery");
    println!("cargo:rustc-link-arg-bin=waku-rust-simple-example=-L../../build/");
}
