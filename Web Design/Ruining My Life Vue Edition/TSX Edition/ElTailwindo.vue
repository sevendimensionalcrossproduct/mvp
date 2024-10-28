<script lang="tsx">
import { defineComponent } from 'vue';

export default defineComponent({
  name: 'ElTailwindo',
  setup() {
    const handleClick = () => {
      alert('penis');
    };

    return { handleClick };
  },

  render() {
    return (
      <div class="max-w-sm mx-auto bg-white rounded-lg shadow-lg overflow-hidden">
        <div class="p-6">
          <h2 class="text-2xl font-bold text-gray-800">Welcome to Tailwind CSS!</h2>
          <p class="mt-2 text-gray-600">
            Tailwind CSS is a utility-first CSS framework for creating custom designs without having to leave your HTML.
          </p>
          <button
            onClick={this.handleClick}
            class="mt-4 w-full bg-blue-500 text-white font-semibold py-2 rounded hover:bg-blue-600 transition duration-200"
          >
            Get Started
          </button>
        </div>
      </div>
    );
  },
});
</script>

<style scoped>
@import 'tailwindcss/base';
@import 'tailwindcss/components';
@import 'tailwindcss/utilities';
</style>
