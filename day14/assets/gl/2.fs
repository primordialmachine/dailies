// "fragment color" input to fragment program.
in vec4 fragmentColor2;

// "fragment color" output from fragment program.
out vec4 fragmentColor3;

void main() {
  fragmentColor3 = fragmentColor2;
}
