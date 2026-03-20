export async function executeCommands(input) {
  try {
    const response = await fetch("http://localhost:3000/execute", {
      method: "POST",
      headers: {
        "Content-Type": "application/json",
      },
      body: JSON.stringify({ input }),
    });

    const data = await response.json();

    return {
      success: true,
      output: data.output,
    };
  } catch (error) {
    return {
      success: false,
      output: `Error al conectar con el backend: ${error.message}`,
    };
  }
}