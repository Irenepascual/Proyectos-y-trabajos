"use client";
import { useForm } from "react-hook-form";
import { useRouter } from "next/navigation";
import { useState } from "react";

// Importa el CSS ajustado
import "/src/app/style-register-login.css";

// Crea una pagina de inicio de sesion para usuarios. Si el inicio de sesion es exitoso, 
// el usuario es redirigido a la pagina principal con sesion. Si ocurre un error 
// durante el proceso de inicio de sesion, se muestra un mensaje de error en la interfaz. 
// Ademas, se proporciona un enlace para que los usuarios sin cuenta se registren.

function LoginUsuarioPage() {
    const { register, handleSubmit, formState: { errors } } = useForm();
    const router = useRouter();
    const [error, setError] = useState(null);

    const onSubmit = handleSubmit(async (data) => {
        console.log(data);

        const res = await fetch('/routes/loginUsuario', {
            method: "POST",
            body: JSON.stringify({
                nom_usuario: data.nom_usuario,
                contrasena: data.contrasena,
            }),
            headers: {
                "Content-Type": "application/json",
            },
        });

        console.log(res);

        if (!res.ok) {
            console.log('error en el login');
            setError(res.error);
            setError({ message: "Error en el login, verifique sus credenciales" });
        } else {
            // Guardar el nombre de usuario en localStorage
            localStorage.setItem('nom_usuario', data.nom_usuario);

            // Login exitoso -> redirigir al usuario a la página home con sesión
            console.log('login exitoso');
            router.push("/home-con-sesion");
            router.refresh();
        }
    });

    return (
        <div className="login-page">
            <form onSubmit={onSubmit} className="login-form">
                {error && (
                    <p className="error-message">
                        {error.message}
                    </p>
                )}

                <h1>Iniciar Sesión Usuario</h1>

                <label>Nombre de usuario:
                    <input
                        type="text"
                        {...register("nom_usuario", {
                            required: {
                                value: true,
                                message: "El nombre de usuario es obligatorio",
                            },
                        })}
                        placeholder="miUsuario123"
                    />
                </label>

                {errors.nom_usuario && (
                    <span className="error-text">
                        {errors.nom_usuario.message}
                        <br />
                    </span>
                )}

                <label>Contraseña:
                    <input
                        type="password"
                        {...register("contrasena", {
                            required: {
                                value: true,
                                message: "La contraseña es obligatoria",
                            },
                        })}
                        placeholder="******"
                    />
                </label>

                {errors.contrasena && (
                    <span className="error-text">
                        {errors.contrasena.message}
                        <br />
                    </span>
                )}

                <button className="login-button">Iniciar Sesión</button>

                <div className="register-link">
                    <button
                        type="button"
                        onClick={() => router.push('/register/usuario')}
                    >
                        ¿No tienes cuenta?
                    </button>
                </div>
            </form>
        </div>
    );
}

export default LoginUsuarioPage;
