export default function Toolbar({
  fileName,
  onFileChange,
  onExecute,
  onClear,
  loading,
}) {
  return (
    <div className="grid gap-4 md:grid-cols-[1fr_auto_auto] items-center mb-6">
      <label className="flex items-center gap-3 rounded-2xl border border-dashed border-gray-300 bg-gray-50 px-4 py-3 cursor-pointer hover:bg-gray-100 transition">
        <input
          type="file"
          accept=".smia,.txt"
          className="hidden"
          onChange={onFileChange}
        />
        <span className="rounded-xl bg-gray-900 px-4 py-2 text-sm font-medium text-white">
          Elegir archivo
        </span>
        <span className="text-sm text-gray-500">
          {fileName || "Ningún archivo seleccionado"}
        </span>
      </label>

      <button
        onClick={onExecute}
        disabled={loading}
        className="rounded-2xl bg-blue-600 px-5 py-3 text-sm font-semibold text-white shadow hover:bg-blue-700 transition disabled:opacity-60"
      >
        {loading ? "Ejecutando..." : "Ejecutar"}
      </button>

      <button
        onClick={onClear}
        disabled={loading}
        className="rounded-2xl bg-gray-200 px-5 py-3 text-sm font-semibold text-gray-800 hover:bg-gray-300 transition disabled:opacity-60"
      >
        Limpiar
      </button>
    </div>
  );
}